`include "Common.vh"

module RWMask(
    input logic [`MAX_INSTR_ID:0] instr_tb,
    input logic [`MAX_TYPE_ID:0] type_tb,
    input logic [14:0] fields,
    output logic [30:0] rmask, wmask
);
    logic [4:0] rs, rt, rd;
    assign rs = fields[14:10];
    assign rt = fields[9:5];
    assign rd = fields[4:0];

    logic [30:0] rs_mask, rt_mask, rd_mask, ra_mask;
    assign rs_mask = (1 << rs) >> 1;
    assign rt_mask = (1 << rt) >> 1;
    assign rd_mask = (1 << rd) >> 1;
    assign ra_mask = 1 << 30;

    logic read_rs, read_rt, write_rt, write_rd, write_ra;
    assign read_rs = type_tb[`RTYPE] | type_tb[`IRTYPE] |
        instr_tb[`SW] | instr_tb[`LW] |
        instr_tb[`BEQ] | instr_tb[`BNE];
    assign read_rt = type_tb[`RTYPE] | instr_tb[`SW] |
        instr_tb[`BEQ] | instr_tb[`BNE];
    assign write_rt = instr_tb[`LW] | type_tb[`IRTYPE];
    assign write_rd = type_tb[`RTYPE];
    assign write_ra = instr_tb[`JAL];

    assign rmask = (read_rs ? rs_mask : 0) | (read_rt ? rt_mask : 0);
    assign wmask = (write_rt ? rt_mask : 0) | (write_rd ? rd_mask : 0) | (write_ra ? ra_mask : 0);

    logic _unused_ok = &{1'b0, instr_tb, type_tb, 1'b0};
endmodule