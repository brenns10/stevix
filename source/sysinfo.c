/**
 * sysinfo.c: Contains sysinfo(), which outputs ARM system control coprocessor
 * information onto the console. Potentially in the future could output memory
 * mappings, and just all sorts of useful tidbits.
 */
#include "pi.h"

/**
 * A handy macro that loads coprocessor info into a variable. Much nicer than
 * having to write an assembly function to do it.
 */
#define get_cpreg(dst, CRn, op1, CRm, op2) \
	__asm__ __volatile__ ( \
		"mrc p15, " #op1 ", %[rd], " #CRn ", " #CRm ", " #op2 \
		: [rd] "=r" (dst) \
		: \
		: \
		)

#define sysinfo_entry(CRn, op1, CRm, op2, fstr) \
	do { \
		get_cpreg(ra, CRn, op1, CRm, op2); \
		printf(fstr ": %x\n", ra); \
	} while (0)


void sysinfo(void)
{
	uint32_t ra;
	/* outputting values for registers documented at:
	 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344k/Beidabib.html
	 */
	sysinfo_entry(c0, 0, c0, 0, "Main ID");
	sysinfo_entry(c0, 0, c0, 1, "Cache Type");
	sysinfo_entry(c0, 0, c0, 2, "TCM Status");
	sysinfo_entry(c0, 0, c0, 3, "TLB Type");
	sysinfo_entry(c0, 0, c1, 0, "Processor Feature 0");
	sysinfo_entry(c0, 0, c1, 1, "Processor Feature 1");
	sysinfo_entry(c0, 0, c1, 2, "Debug Feature 0");
	sysinfo_entry(c0, 0, c1, 3, "Auxiliary Feature 0");
	sysinfo_entry(c0, 0, c1, 4, "Memory Model Feature 0");
	sysinfo_entry(c0, 0, c1, 5, "Memory Model Feature 1");
	sysinfo_entry(c0, 0, c1, 6, "Memory Model Feature 2");
	sysinfo_entry(c0, 0, c1, 7, "Memory Model Feature 3");
	sysinfo_entry(c0, 0, c2, 0, "Instruction Set Feature Attribute 0");
	sysinfo_entry(c0, 0, c2, 1, "Instruction Set Feature Attribute 1");
	sysinfo_entry(c0, 0, c2, 2, "Instruction Set Feature Attribute 2");
	sysinfo_entry(c0, 0, c2, 3, "Instruction Set Feature Attribute 3");
	sysinfo_entry(c0, 0, c2, 4, "Instruction Set Feature Attribute 4");
	sysinfo_entry(c0, 0, c2, 5, "Instruction Set Feature Attribute 5");

	sysinfo_entry(c1, 0, c0, 0, "Control");
	sysinfo_entry(c1, 0, c0, 1, "Auxiliary Control");
	sysinfo_entry(c1, 0, c0, 2, "Coprocessor Access Control");
	sysinfo_entry(c1, 0, c1, 0, "Secure Configuration");
	sysinfo_entry(c1, 0, c1, 1, "Secure Debug Enable");
	sysinfo_entry(c1, 0, c1, 2, "Non-Secure Access Control");

	sysinfo_entry(c2, 0, c0, 0, "Translation Table Base 0");
	sysinfo_entry(c2, 0, c0, 1, "Translation Table Base 1");
	sysinfo_entry(c2, 0, c0, 2, "Translation Table Base Control");

	sysinfo_entry(c3, 0, c0, 0, "Domain Access Control");

	sysinfo_entry(c5, 0, c0, 0, "Data Fault Status");
	sysinfo_entry(c5, 0, c0, 1, "Instruction Fault Status");

	sysinfo_entry(c6, 0, c0, 1, "Fault Address");;
	sysinfo_entry(c6, 0, c0, 1, "Watchpoint Fault Address");;
	sysinfo_entry(c6, 0, c0, 2, "Instruction Fault Address");;

	/* most of c7 is WO */
	sysinfo_entry(c7, 0, c4, 0, "PA");
	sysinfo_entry(c7, 0, c10, 6, "Cache Dirty Status");

	/* all of c8 is WO */

	sysinfo_entry(c9, 0, c0, 0, "Data Cache Lockdown");
	sysinfo_entry(c9, 0, c0, 1, "Instruction Cache Lockdown");
	sysinfo_entry(c9, 0, c1, 0, "Data TCM Region");
	sysinfo_entry(c9, 0, c1, 1, "Instruction TCM Region");
	sysinfo_entry(c9, 0, c1, 2, "Data TCM Non-secure Control Access");
	sysinfo_entry(c9, 0, c1, 3, "Instruction TCM Non-secure Control Access");
	sysinfo_entry(c9, 0, c2, 0, "TCM Selection");
	sysinfo_entry(c9, 0, c8, 0, "Cache Behavior Override");

	sysinfo_entry(c10, 0, c0, 0, "TLB Lockdown");
	sysinfo_entry(c10, 0, c2, 0, "Primary Region Memory Remap Register");
	sysinfo_entry(c10, 0, c2, 1, "Normal Memory Region Remap Register");

	sysinfo_entry(c11, 0, c0, 0, "DMA Identification and Status (0)");
	sysinfo_entry(c11, 0, c0, 1, "DMA Identification and Status (1)");
	sysinfo_entry(c11, 0, c0, 2, "DMA Identification and Status (2)");
	sysinfo_entry(c11, 0, c0, 3, "DMA Identification and Status (3)");
	sysinfo_entry(c11, 0, c1, 0, "DMA User Accessibility");
	sysinfo_entry(c11, 0, c2, 0, "DMA Channel Number");
	sysinfo_entry(c11, 0, c4, 0, "DMA Control");
	sysinfo_entry(c11, 0, c5, 0, "DMA Internal Start Address");
	sysinfo_entry(c11, 0, c6, 0, "DMA External Start Address");
	sysinfo_entry(c11, 0, c7, 0, "DMA Internal End Address");
	sysinfo_entry(c11, 0, c8, 0, "DMA Channel Status");
	sysinfo_entry(c11, 0, c15, 0, "DMA Context ID");

	sysinfo_entry(c12, 0, c0, 0, "Secure or Non-secure Vector Base Address");
	sysinfo_entry(c12, 0, c0, 1, "Monitor Vector Base Address");
	sysinfo_entry(c12, 0, c1, 0, "Interrupt Status");

	sysinfo_entry(c13, 0, c0, 0, "FCSE PID");
	sysinfo_entry(c13, 0, c0, 1, "Context ID");
	sysinfo_entry(c13, 0, c0, 2, "User Read/Write Thread and Process ID");
	sysinfo_entry(c13, 0, c0, 3, "User Read-only Thread and Process ID");
	sysinfo_entry(c13, 0, c0, 4, "Privileged Only Thread and Process ID");

	sysinfo_entry(c15, 0, c2, 4, "Peripheral Port Memory Remap");
	sysinfo_entry(c15, 0, c9, 0, "Secure User and Non-secure Access Validation Control");
	sysinfo_entry(c15, 0, c12, 0, "Performance Monitor Control");
	sysinfo_entry(c15, 0, c12, 1, "Cycle Counter");
	sysinfo_entry(c15, 0, c12, 2, "Count 0");
	sysinfo_entry(c15, 0, c12, 3, "Count 1");
	sysinfo_entry(c15, 0, c12, 4, "System Validation Counter (0)");
	sysinfo_entry(c15, 0, c12, 5, "System Validation Counter (1)");
	sysinfo_entry(c15, 0, c12, 6, "System Validation Counter (2)");
	sysinfo_entry(c15, 0, c12, 7, "System Validation Counter (3)");
	sysinfo_entry(c15, 0, c13, 0, "System Validation Operations (0)");
	sysinfo_entry(c15, 0, c13, 1, "System Validation Operations (1)");
	sysinfo_entry(c15, 0, c13, 2, "System Validation Operations (2)");
	sysinfo_entry(c15, 0, c13, 3, "System Validation Operations (3)");
	sysinfo_entry(c15, 0, c13, 4, "System Validation Operations (4)");
	sysinfo_entry(c15, 0, c13, 5, "System Validation Operations (5)");
	sysinfo_entry(c15, 0, c13, 6, "System Validation Operations (6)");
	sysinfo_entry(c15, 0, c13, 7, "System Validation Operations (7)");
	/* i don't feel like doing the rest */
}
