/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:*
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file test_l1_dhcp6cApi.c
 * @page dhcp6cApi Level 1 Tests
 *
 * ## Module's Role
 * This module includes Level 1 functional tests (success and failure scenarios).
 * This is to ensure that the dhcp6cApi APIs meet the requirements across all vendors.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
 */
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dhcp6cApi.h"

#define UT_KVP_MAX_ELEMENT_SIZE 32

static int gTestGroup = 1;
static int gTestID = 1;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS 0
#endif

#ifndef STATUS_FAILURE
#define STATUS_FAILURE -1
#endif

#ifndef INT32_MAX
#define INT32_MAX 0x7FFFFFFF
#endif

#ifndef INT32_MIN
#define INT32_MIN (-INT32_MAX - 1)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 0xFFFFFFFFU
#endif

dhcp6cInfo_t pInfo;

int ValdtIpV6(unsigned char *Ipv6, int elmnt)
{
    int i = 0;
    char ip_string[64] = {'\0'};

    if (Ipv6 != NULL || Ipv6[0] != '\0')
    {
        sprintf(ip_string, "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", Ipv6[0], Ipv6[1], Ipv6[2], Ipv6[3], Ipv6[4], Ipv6[5], Ipv6[6], Ipv6[7], Ipv6[8], Ipv6[9], Ipv6[10], Ipv6[11], Ipv6[12], Ipv6[13], Ipv6[14], Ipv6[15]);
        UT_LOG_DEBUG("%s", ip_string);
        for (i = 0; i < elmnt; i++)
        {
            if (Ipv6[i] > 0xff)
                break;
        }

        if (i == elmnt)
            return 1;
    }

    return 0;
}

int null_check(char *string_data)
{
    if (string_data != NULL || string_data[0] != '\0')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Test the function ert_dhcp6c_get_info to verify if it returns the expected status.
 *
 * This test case verifies the functionality of the function ert_dhcp6c_get_info by invoking it with valid memory address for pInfo and checking if the returned status is equal to STATUS_SUCCESS.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 001
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ert_dhcp6c_get_info with valid memory address | pInfo = valid memory address | STATUS_SUCCESS | Should be successful |
 */
void test_l1_dhcp6cApi_positive1_ert_dhcp6c_get_info(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int status = 0;
    int flg = 0;
    int i = 0;
    memset(&pInfo, 0, sizeof(dhcp6cInfo_t));

    UT_LOG_DEBUG("Invoking ert_dhcp6c_get_info with pInfo = valid memory address");
    status = ert_dhcp6c_get_info(&pInfo);
    UT_LOG_DEBUG("return status : %d ", status);
    UT_LOG_DEBUG("Validating ifname...");

    flg = null_check(pInfo.ifname);
    if (flg == 1)
    {
        UT_LOG_DEBUG("pInfo.ifname: %s", pInfo.ifname);
        UT_ASSERT_KVP_EQUAL_PROFILE_STRING(pInfo.ifname, "dhcp6c/config/ERouter_DHCPv6/ifName");
    }
    else
    {
        UT_LOG_DEBUG("ifname  is empty string");
        UT_FAIL("ifname  is empty string");
    }

    UT_LOG("Validating smState...");
    UT_LOG("pInfo.smState:%d\n", pInfo.smState);
    UT_ASSERT(pInfo.smState >= 0);

    UT_LOG("Validating configAttemps...");
    UT_LOG("pInfo.configAttemps:%d\n", pInfo.configAttemps);
    UT_ASSERT(pInfo.configAttemps >= 0);

    UT_LOG_DEBUG("Validating ipAddr");
    flg = ValdtIpV6(pInfo.ipAddr, (sizeof(pInfo.ipAddr) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating prefix ");
    flg = ValdtIpV6(pInfo.prefix, (sizeof(pInfo.prefix) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating gw ");
    flg = ValdtIpV6(pInfo.gw, (sizeof(pInfo.gw) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating dhcpServer ");
    flg = ValdtIpV6(pInfo.dhcpServer, (sizeof(pInfo.dhcpServer) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating t1...");
    UT_LOG_DEBUG("pInfo.t1:%u\n", pInfo.t1);
    UT_ASSERT(pInfo.t1 >= 0);

    UT_LOG_DEBUG("Validating t2...");
    UT_LOG_DEBUG("pInfo.t2:%u\n", pInfo.t2);
    UT_ASSERT(pInfo.t2 >= 0);

    UT_LOG_DEBUG("Validating leasetime...");
    UT_LOG_DEBUG("pInfo.leaseTime:%u\n", pInfo.leaseTime);
    UT_ASSERT(pInfo.leaseTime >= 0);

    UT_LOG_DEBUG("Validating leaseGottime...");
    UT_LOG_DEBUG("pInfo.leaseGotTime:%u\n", pInfo.leaseGotTime);
    UT_ASSERT(pInfo.leaseGotTime >= 0);

    UT_LOG_DEBUG("Validating remainLeaseTime...");
    UT_LOG_DEBUG("pInfo.remainLeaseTime:%u\n", pInfo.remainLeaseTime);
    UT_ASSERT(pInfo.remainLeaseTime >= 0);

    UT_LOG_DEBUG("Validating remainRenewTime...");
    UT_LOG_DEBUG("pInfo.remainRenewTime:%u\n", pInfo.remainRenewTime);
    UT_ASSERT(pInfo.remainRenewTime >= 0);

    UT_LOG_DEBUG("Validating remainRebindTime...");
    UT_LOG_DEBUG("pInfo.remainRebindTime:%u\n", pInfo.remainRebindTime);
    UT_ASSERT(pInfo.remainRebindTime >= 0);

    UT_LOG("Validating numOfDns...");
    UT_LOG("pInfo.numOfDns:%d\n", pInfo.numOfDns);
    UT_ASSERT(pInfo.numOfDns >= 0);

    UT_LOG_DEBUG("dnsAddrList...");
    for (i = 0; i < MAX_IPV6_ADDR_LIST_SIZE; i++)
    {
        flg = ValdtIpV6(pInfo.dnsAddrList[i], IPV6_ADDR_SIZE);
        UT_LOG_DEBUG("flg:%d\n", flg);
        UT_ASSERT(flg == 1);
    }

    UT_ASSERT_EQUAL(status, STATUS_SUCCESS);
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test is to verify the negative scenario when ert_dhcp6c_get_info is invoked with pInfo being a NULL pointer.
 *
 * The objective of this test is to ensure that the function behaves correctly when pInfo is passed as a NULL pointer.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 002
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console.
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke ert_dhcp6c_get_info with pInfo as NULL pointer | pInfo = NULL | STATUS_FAILURE | Should fail |
 */
void test_l1_dhcp6cApi_negative1_ert_dhcp6c_get_info(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int status = 0;
    dhcp6cInfo_t *pInfo = NULL;

    UT_LOG_DEBUG("Invoking ert_dhcp6c_get_info with pInfo = NULL");
    status = ert_dhcp6c_get_info(pInfo);
    UT_LOG_DEBUG("Expected Result: STATUS_FAILURE");
    UT_LOG_DEBUG("Actual Result: %d", status);

    UT_ASSERT_EQUAL(status, STATUS_FAILURE);
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief Test case to verify the functionality of ecm_dhcp6c_get_info function.
 *
 * This test case verifies the functionality of ecm_dhcp6c_get_info function by invoking it and checking the returned result.
 *
 * **Test Group ID:** Basic: 02
 * **Test Case ID:** 003
 * **Priority:** High
 *
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console
 *
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ecm_dhcp6c_get_info with valid memory address | pInfo = valid memory address | STATUS_SUCCESS | Should be successful |
 */
void test_l1_dhcp6cApi_positive_1_ecm_dhcp6c_get_info(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int flg = 0;
    int i = 0;
    int result = 0;

    memset(&pInfo, 0, sizeof(dhcp6cInfo_t));

    UT_LOG_DEBUG("Invoking ecm_dhcp6c_get_info with pInfo = %p", &pInfo);
    result = ecm_dhcp6c_get_info(&pInfo);
    UT_LOG_DEBUG("Expected Result: STATUS_SUCCESS");
    UT_LOG_DEBUG("Actual Result: %d", result);

    UT_LOG_DEBUG("Validating ifname...");
    flg = null_check(pInfo.ifname);
    if (flg == 1)
    {
        UT_LOG_DEBUG("pInfo.ifname: %s", pInfo.ifname);
        UT_ASSERT_KVP_EQUAL_PROFILE_STRING(pInfo.ifname, "dhcp6c/config/ecm_DHCPv6/ifName");
    }
    else
    {
        UT_LOG_DEBUG("ifname  is empty string");
        UT_FAIL("ifname  is empty string");
    }

    UT_LOG("Validating smState...");
    UT_LOG("pInfo.smState:%d\n", pInfo.smState);
    UT_ASSERT(pInfo.smState >= 0);

    UT_LOG("Validating configAttemps...");
    UT_LOG("pInfo.configAttemps:%d\n", pInfo.configAttemps);
    UT_ASSERT(pInfo.configAttemps >= 0);

    UT_LOG_DEBUG("Validating ipAddr ");
    flg = ValdtIpV6(pInfo.ipAddr, (sizeof(pInfo.ipAddr) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating prefix ");
    flg = ValdtIpV6(pInfo.prefix, (sizeof(pInfo.prefix) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating gw ");
    flg = ValdtIpV6(pInfo.gw, (sizeof(pInfo.gw) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating dhcpServer ");
    flg = ValdtIpV6(pInfo.dhcpServer, (sizeof(pInfo.dhcpServer) / sizeof(unsigned char)));
    UT_LOG_DEBUG("flg:%d\n", flg);
    UT_ASSERT(flg == 1);

    UT_LOG_DEBUG("Validating t1...");
    UT_LOG_DEBUG("pInfo.t1:%u\n", pInfo.t1);
    UT_ASSERT(pInfo.t1 >= 0);

    UT_LOG_DEBUG("Validating t2...");
    UT_LOG_DEBUG("pInfo.t2: %u\n", pInfo.t2);
    UT_ASSERT(pInfo.t2 >= 0);

    UT_LOG_DEBUG("Validating leasetime...");
    UT_LOG_DEBUG("pInfo.leaseTime:%u\n", pInfo.leaseTime);
    UT_ASSERT(pInfo.leaseTime >= 0);

    UT_LOG_DEBUG("Validating leaseGottime...");
    UT_LOG_DEBUG("pInfo.leaseGotTime:%u\n", pInfo.leaseGotTime);
    UT_ASSERT(pInfo.leaseGotTime >= 0);

    UT_LOG_DEBUG("Validating remainLeaseTime...");
    UT_LOG_DEBUG("pInfo.remainLeaseTime:%u\n", pInfo.remainLeaseTime);
    UT_ASSERT(pInfo.remainLeaseTime >= 0);

    UT_LOG_DEBUG("Validating remainRenewTime...");
    UT_LOG_DEBUG("pInfo.remainRenewTime:%u\n", pInfo.remainRenewTime);
    UT_ASSERT(pInfo.remainRenewTime >= 0);

    UT_LOG_DEBUG("Validating remainRebindTime...");
    UT_LOG_DEBUG("pInfo.remainRebindTime:%u\n", pInfo.remainRebindTime);
    UT_ASSERT(pInfo.remainRebindTime >= 0);

    UT_LOG("Validating numOfDns...");
    UT_LOG("pInfo.numOfDns:%d\n", pInfo.numOfDns);
    UT_ASSERT(pInfo.numOfDns >= 0);

    UT_LOG_DEBUG("dnsAddrList...");
    for (i = 0; i < MAX_IPV6_ADDR_LIST_SIZE; i++)
    {
        flg = ValdtIpV6(pInfo.dnsAddrList[i], IPV6_ADDR_SIZE);
        UT_LOG_DEBUG("flg:%d\n", flg);
        UT_ASSERT(flg == 1);
    }

    UT_ASSERT_EQUAL(result, STATUS_SUCCESS);
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify negative scenario of ecm_dhcp6c_get_info function
 *
 * This test case verifies the failure scenario of the ecm_dhcp6c_get_info function by passing NULL as the pInfo argument.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 004
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console
 *
 * **Test Procedure:**
 * 01. Invoke ecm_dhcp6c_get_info function with pInfo = NULL.
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ecm_dhcp6c_get_info with pInfo as NULL pointer | pInfo = NULL | STATUS_FAILURE | Should fail |
 */
void test_l1_dhcp6cApi_negative_1_ecm_dhcp6c_get_info(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result = 0;
    dhcp6cInfo_t *pInfo = NULL;

    UT_LOG_DEBUG("Invoking ecm_dhcp6c_get_info with pInfo = NULL");
    result = ecm_dhcp6c_get_info(pInfo);

    UT_LOG_DEBUG("Expected Result: STATUS_FAILURE");
    UT_LOG_DEBUG("Actual Result: %d", result);

    UT_ASSERT_EQUAL(result, STATUS_FAILURE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t *pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int register_hal_tests(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L1 dhcp6cApi]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }

    UT_add_test(pSuite, "l1_dhcp6cApi_positive1_ert_dhcp6c_get_info", test_l1_dhcp6cApi_positive1_ert_dhcp6c_get_info);
    UT_add_test(pSuite, "l1_dhcp6cApi_negative1_ert_dhcp6c_get_info", test_l1_dhcp6cApi_negative1_ert_dhcp6c_get_info);
    UT_add_test(pSuite, "l1_dhcp6cApi_positive_1_ecm_dhcp6c_get_info", test_l1_dhcp6cApi_positive_1_ecm_dhcp6c_get_info);
    UT_add_test(pSuite, "l1_dhcp6cApi_negative_1_ecm_dhcp6c_get_info", test_l1_dhcp6cApi_negative_1_ecm_dhcp6c_get_info);

    return 0;
}
