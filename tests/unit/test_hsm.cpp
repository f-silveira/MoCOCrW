/*
 * #%L
 * %%
 * Copyright (C) 2022 BMW Car IT GmbH
 * %%
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * #L%
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <string>

// #include "key.cpp"
#include "mococrw/hsm.h"
#include "openssl_lib_mock.h"

using namespace ::mococrw;
using namespace ::std::string_literals;

using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::StrEq;

class HSMTest : public ::testing::Test
{
public:
    void SetUp() override;
    void TearDown() override;

protected:
    std::string _defaultErrorMessage{"bla bla bla"};
    const unsigned long _defaultErrorCode = 1L;
    openssl::OpenSSLLibMock& _mock() const
    {
        return openssl::OpenSSLLibMockManager::getMockInterface();
    }
};

namespace testutils
{
ENGINE* someEnginePtr()
{
    /* Reserve some memory and cast a pointer to that ; pointers will not be dereferenced */
    static char dummyBuf[42] = {};
    return reinterpret_cast<ENGINE*>(&dummyBuf);
}
}  // namespace testutils

void HSMTest::SetUp()
{
    /*
     * Like test_opensslwrapper.cpp, we instrument the mock, so that error-handling code
     * will always have well-defined behavior.
     */
    openssl::OpenSSLLibMockManager::resetMock();
    ON_CALL(_mock(), SSL_ERR_get_error()).WillByDefault(Return(_defaultErrorCode));
    ON_CALL(_mock(), SSL_ERR_error_string(_, nullptr))
            .WillByDefault(Return(const_cast<char*>(_defaultErrorMessage.c_str())));
    // TODO: Get rid of the uninteresting calls by default here somehow...
}

void HSMTest::TearDown() { openssl::OpenSSLLibMockManager::destroy(); }

TEST_F(HSMTest, testHSMEngine)
{
    std::string engineID("engine_id");
    std::string modulePath("/test_path.so");
    std::string pin("1234");

    EXPECT_CALL(_mock(), SSL_ENGINE_by_id(StrEq(engineID.c_str())))
            .WillOnce(Return(::testutils::someEnginePtr()));

    EXPECT_CALL(_mock(),
                SSL_ENGINE_ctrl_cmd_string(::testutils::someEnginePtr(),
                                           StrEq("MODULE_PATH"),
                                           StrEq(modulePath.c_str()),
                                           0 /*non-optional*/))
            .WillOnce(Return(1));

    EXPECT_CALL(_mock(),
                SSL_ENGINE_ctrl_cmd_string(::testutils::someEnginePtr(),
                                           StrEq("PIN"),
                                           StrEq(pin.c_str()),
                                           0 /*non-optional*/))
            .WillOnce(Return(1));

    EXPECT_CALL(_mock(), SSL_ENGINE_init(::testutils::someEnginePtr())).WillOnce(Return(1));

    EXPECT_CALL(_mock(), SSL_ENGINE_finish(::testutils::someEnginePtr())).WillOnce(Return(1));

    EXPECT_NO_THROW((HsmEngine(engineID, modulePath, pin)));
}
