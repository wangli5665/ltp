#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2024-2025 Cyril Hrubis <chrubis@suse.cz>
#
# ---
# env
# {
# }
# ---

TST_CLEANUP=do_cleanup

do_cleanup()
{
	tst_res TINFO "Cleanup executed"
}

tst_test()
{
	tst_res TPASS "Test is executed"
}

. tst_loader.sh
