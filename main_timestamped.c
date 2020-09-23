/*
 * Copyright (c) 2019,2020 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <arm_cmse.h>
#include "tfm_api.h"
#include "tfm_ns_interface.h"
#ifdef TFM_PSA_API
#include "psa_manifest/sid.h"
#include <stdlib.h>
#endif


uint32_t  test1001;
uint32_t  test1002;
uint32_t  test1003;
uint32_t  test1004;
uint32_t  test1005;
uint32_t  test1006;

K_MUTEX_DEFINE(tfm_mutex);


__attribute__ ((no_instrument_function))
void __cyg_profile_func_enter(void *this, void *call_site)
{


	__asm__ volatile(
	"mov   r0,lr \n\t"
	"bl enter_veneer");


}

__attribute__ ((no_instrument_function))
void __cyg_profile_func_exit(void *this, void *call_site)
	{

			__asm__ volatile(
			"mov r0,lr \n\t"
			"bl exit_veneer \n\t");


	}

/*
__attribute__ ((no_instrument_function))
s_stack __attribute__((cmse_nonsecure_entry,noclone)) make_stack()
{
	s_stack my_stack;
	my_stack.top =-1;
	return my_stack;
}
__attribute__ ((no_instrument_function))
void __attribute__((cmse_nonsecure_entry,noclone)) push_stack(s_stack *pstack, uint32_t lr)
{
	pstack->top +=1;
	pstack->stack[pstack->top] = lr;

}
__attribute__ ((no_instrument_function))
void __attribute__((cmse_nonsecure_entry,noclone)) pop_stack(s_stack *pstack, uint32_t lr)
{

	if (pstack->stack[pstack->top] == lr){
	pstack->top-=1;
	}




}
*/



__attribute__ ((no_instrument_function))
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
				  uint32_t arg0, uint32_t arg1,
				  uint32_t arg2, uint32_t arg3)
{
	int32_t result;

	/* TFM request protected by NS lock */
	if (k_mutex_lock(&tfm_mutex, K_FOREVER) != 0) {
		return (int32_t)TFM_ERROR_GENERIC;
	}

	result = fn(arg0, arg1, arg2, arg3);

	k_mutex_unlock(&tfm_mutex);

	return result;
}
__attribute__ ((no_instrument_function))
enum tfm_status_e tfm_ns_interface_init(void)
{
	/* The static K_MUTEX_DEFINE handles mutex init, so just return. */

	return TFM_SUCCESS;
}

/**
 * \brief Retrieve the version of the PSA Framework API.
 *
 * \note This is a functional test only and doesn't
 *       mean to test all possible combinations of
 *       input parameters and return values.
 */
static void tfm_ipc_test_1001(void)
{
  u32_t  tic = k_uptime_get_32();
	uint32_t version;

	version = psa_framework_version();
	if (version == PSA_FRAMEWORK_VERSION) {
		printk("The version of the PSA Framework API is %d.\n",version);
           uint32_t  toc = k_uptime_get_32();
           test1001 = (toc-tic);
	} else {
		printk("The version of the PSA Framework API is not valid!\n");
    uint32_t  toc = k_uptime_get_32();
    test1001 = (toc-tic);
		return;
	}

}



/**
 * Retrieve the minor version of a RoT Service.
 *
 * @return N/A
 */

static void tfm_ipc_test_1002(void)
{
  uint32_t  tic = k_uptime_get_32();
	uint32_t version;

	version = psa_version(IPC_SERVICE_TEST_BASIC_SID);
	if (version == PSA_VERSION_NONE) {
		printk("RoT Service is not implemented or caller is not ");
		printk("authorized to access it!\n");
    uint32_t  toc = k_uptime_get_32();
    test1002 =(toc-tic);
		return;
	}

	/* Valid version number */
	printk("The minor version is %d.\n", version);
	uint32_t  toc = k_uptime_get_32();
    	test1002 =(toc-tic);
}

/**
 * Connect to a RoT Service by its SID.
 *
 * @return N/A
 */
static void tfm_ipc_test_1003(void)
{
  uint32_t  tic = k_uptime_get_32();
	psa_handle_t handle;

	handle = psa_connect(IPC_SERVICE_TEST_BASIC_SID,
			     IPC_SERVICE_TEST_BASIC_VERSION);
	if (handle > 0) {
		printk("Connect success!\n");
    uint32_t  toc = k_uptime_get_32();
    test1003 = (toc-tic);
	} else {
		printk("The RoT Service has refused the connection!\n");
    uint32_t  toc = k_uptime_get_32();
    test1003 = (toc-tic);
		return;
	}
	psa_close(handle);
}

/**
 * Call a RoT Service.
 *
 * @return N/A
 */
static void tfm_ipc_test_1004(void)
{
  uint32_t  tic = k_uptime_get_32();
	char str1[] = "str1";
	char str2[] = "str2";
	char str3[128], str4[128];
	struct psa_invec invecs[2] = { { str1, sizeof(str1) /
					 sizeof(char) },
				       { str2, sizeof(str2) /
					 sizeof(char) } };
	struct psa_outvec outvecs[2] = { { str3, sizeof(str3) /
					   sizeof(char) },
					 { str4, sizeof(str4) /
					   sizeof(char) } };
	psa_handle_t handle;
	psa_status_t status;
	uint32_t version;

	version = psa_version(IPC_SERVICE_TEST_BASIC_SID);
	printk("TFM service support version is %d.\n", version);
	handle = psa_connect(IPC_SERVICE_TEST_BASIC_SID,
			     IPC_SERVICE_TEST_BASIC_VERSION);
	status = psa_call(handle, PSA_IPC_CALL, invecs, 2, outvecs, 2);
	if (status >= 0) {
		printk("psa_call is successful!\n");
    uint32_t  toc = k_uptime_get_32();
    test1004 = (toc-tic);
	} else {
		printk("psa_call is failed!\n");
    uint32_t  toc = k_uptime_get_32();
    test1004 = (toc-tic);
		return;
	}

	printk("outvec1 is: %s\n", (char *)(outvecs[0].base));
	printk("outvec2 is: %s\n", (char *)(outvecs[1].base));
	psa_close(handle);
}

/**
 * \brief Call IPC_CLIENT_TEST_BASIC_SID RoT Service to run the IPC basic test.
 */
static void tfm_ipc_test_1005(void)
{
  uint32_t  tic = k_uptime_get_32();
	psa_handle_t handle;
	psa_status_t status;
	int test_result;
	struct psa_outvec outvecs[1] = { { &test_result,
					   sizeof(test_result) } };

	handle = psa_connect(IPC_CLIENT_TEST_BASIC_SID,
			     IPC_CLIENT_TEST_BASIC_VERSION);
	if (handle > 0) {
		printk("Connect success!\n");
    uint32_t  toc = k_uptime_get_32();
    test1005 = (double)(toc-tic);
	} else {
		printk("The RoT Service has refused the connection!\n");
    uint32_t  toc = k_uptime_get_32();
    test1005 = (toc-tic);
		return;
	}

	status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
	if (status >= 0) {
		printk("Call success!\n");
	} else {
		printk("Call failed!\n");
	}

	psa_close(handle);
}

/**
 * \brief Call IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID RoT Service
 *  to run the IPC PSA access APP mem test.
 */
static void tfm_ipc_test_1006(void)
{
  uint32_t  tic = k_uptime_get_32();
	psa_handle_t handle;
	psa_status_t status;
	int test_result;
	struct psa_outvec outvecs[1] = { { &test_result,
					   sizeof(test_result) } };

	handle = psa_connect(
		IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID,
		IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_VERSION);
	if (handle > 0) {
		printk("Connect success!\n");
    uint32_t  toc = k_uptime_get_32();
    test1006 = (toc-tic);
	} else {
		printk("The RoT Service has refused the connection!\n");
    uint32_t  toc = k_uptime_get_32();
    test1006 = (toc-tic);
		return;
	}

	status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
	if (status >= 0) {
		printk("Call success!\n");
	} else {
		printk("Call failed!\n");
	}

	psa_close(handle);
}
int *arr[100];
int top = -1;
__attribute__ ((no_instrument_function))
void add(int *ar,int t){
top++;
arr[top] = 0x0234D;
}

void main(void)
{


	add(arr,top);
	tfm_ipc_test_1001();
  printf("Test 1001 = %" PRIu32 "\n", test1001);

	tfm_ipc_test_1002();
  printf("Test 1002 = %" PRIu32 "\n", test1002);
	tfm_ipc_test_1003();
  printf("Test 1003 = %" PRIu32 "\n", test1003);
	tfm_ipc_test_1004();
  printf("Test 1004 = %" PRIu32 "\n", test1004);
	tfm_ipc_test_1005();
  printf("Test 1005 = %" PRIu32 "\n", test1005);
	tfm_ipc_test_1006();
  printf("Test 1006 = %" PRIu32 "\n", test1006);

	printk("TF-M IPC on %s\n", CONFIG_BOARD);
}
