#pragma once

/* ------------------- ENABLE TEST BENCH  -------------------*/

// Allow to test a subfunction or part of the program
// and doesn't enter into the state machine
#define TEST_BENCH_ENABLE 0


// Works as the regular setup and loop functions

/* ------------------- ENABLE TEST BENCH  -------------------*/

void bench_loop();

void bench_setup();

void test_bench();