`ifndef CACHE_VH
`define CACHE_VH

`timescale 1ns / 1ps

`define ADDR_WIDTH 32

// tag bits
`define CACHE_T 26
// set index bits
`define CACHE_S 2
// block offset bits
`define CACHE_B 4
// The sum of above bits should be 32 (ADDR_WIDTH)

// number of lines per set
`define CACHE_E 4

`define SET_NUM 2**`CACHE_S
`define BLOCK_SIZE 2**`CACHE_B

`endif
