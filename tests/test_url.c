#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <unistd.h>
#include "dcap_url.h"
#include "dcap_types.h"

extern void dc_setDebugLevel(unsigned int);

int init_suite(void)
{
    dc_setDebugLevel(0);
    return 0;
}

int clean_suite(void)
{
    return 0;
}

void test_not_a_url()
{
    dcap_url * url = dc_getURL("/path/to/file");
    CU_ASSERT_PTR_NULL(url);
}

void test_url()
{
    dcap_url * url =  dc_getURL("dcap://door.site.org:22127/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "door.site.org");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, 22127);
}

void test_url_with_prefix()
{
    dcap_url * url = dc_getURL("gsidcap://door.site.org:22127/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "door.site.org");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, 22127);
    CU_ASSERT_STRING_EQUAL(url->prefix, "gsi");
}

void test_url_no_port()
{
    dcap_url * url = dc_getURL("dcap://door.site.org/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "door.site.org");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, DEFAULT_DOOR_PORT);
}

void test_url_ipv4()
{
    dcap_url * url = dc_getURL("dcap://1.2.3.4:22127/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "1.2.3.4");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, 22127);
}

void test_url_ipv4_no_port()
{
    dcap_url * url = dc_getURL("dcap://1.2.3.4/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "1.2.3.4");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, DEFAULT_DOOR_PORT);
}

void test_url_ipv6()
{
    dcap_url * url = dc_getURL("dcap://[fe80::21c:c0ff:fea0:caf4]:22127/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "fe80::21c:c0ff:fea0:caf4");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, 22127);
}

void test_url_ipv6_no_port()
{
    dcap_url * url = dc_getURL("dcap://[fe80::21c:c0ff:fea0:caf4]/path/to/file");
    CU_ASSERT_PTR_NOT_NULL(url);
    CU_ASSERT_EQUAL(url->type, URL_DCAP);
    CU_ASSERT_STRING_EQUAL(url->host, "fe80::21c:c0ff:fea0:caf4");
    CU_ASSERT_STRING_EQUAL(url->file, "path/to/file");
    CU_ASSERT_EQUAL(url->port, 22125);
}

void test_bad_formated()
{
    dcap_url * url = dc_getURL("dcap://[fe80::21c:c0ff:fea0:caf4/path/to/file");
    CU_ASSERT_PTR_NULL(url);
}

void test_escape_non_ascii() {
  dcap_url * url = dc_getURL("dcap://1.2.3.4/path/to/some_ü_file");
  CU_ASSERT_STRING_EQUAL(url->file, "path/to/some_%C3%BC_file");
}

void test_escape_space() {
  dcap_url * url = dc_getURL("dcap://1.2.3.4/path/to/some file");
  CU_ASSERT_STRING_EQUAL(url->file, "path/to/some%20file");
}

int main()
{
    CU_pSuite pSuite = NULL;
    unsigned int    fails;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("dcap_url", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (
            (NULL == CU_add_test(pSuite, "not a url", test_not_a_url))
            ||
            (NULL == CU_add_test(pSuite, "well formated url", test_url))
            ||
            (NULL == CU_add_test(pSuite, "well formated url with prefix", test_url_with_prefix))
            ||
            (NULL == CU_add_test(pSuite, "url without port number", test_url_no_port))
            ||
            (NULL == CU_add_test(pSuite, "url with ipv4", test_url_ipv4))
            ||
            (NULL == CU_add_test(pSuite, "url with ipv4, no port number", test_url_ipv4_no_port))
            ||
            (NULL == CU_add_test(pSuite, "url with ipv6", test_url_ipv6))
            ||
            (NULL == CU_add_test(pSuite, "url with ipv6, no port", test_url_ipv6_no_port))
            ||
            (NULL == CU_add_test(pSuite, "bad formated url", test_bad_formated))
            ||
            (NULL == CU_add_test(pSuite, "non ascii url not encoded", test_escape_non_ascii))
            ||
            (NULL == CU_add_test(pSuite, "space in url not encoded", test_escape_space))
            ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    fails = CU_get_number_of_tests_failed();
    CU_cleanup_registry();
    return fails;
}
