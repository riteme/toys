module Datapath(
    input logic clk, reset,
    input logic [31:0] imem_out, mem_out,
    output logic mem_en,
    output logic [31:0] iaddr, addr, mem_data
);
    logic [31:0] pc0, instr0;

    logic [4:0] rd1, sht1;
    logic [31:0] imm1, vs1, vt1;
    logic [8:0] signal1;

    logic [4:0] rd2;
    logic [31:0] out2, vt2;
    logic [1:0] signal2;

    logic [31:0] F_pc, F_instr;
    Frontend frontend(
        .clk(clk), .stall(stall), .reset(reset), .eq(D_eq),
        .data(imem_out), .prev(instr0), .prev_pc(pc0),
        .vs(D_vs), .iaddr(iaddr),
        .pc(F_pc), .instr(F_instr)
    );

    logic [31:0] v1, v2;  // controlled by HazardController
    logic [31:0] v1_reg, v2_reg;
    RegisterFile regfile(
        .clk(clk), .reset(reset),
        .r1(D_r1), .r2(D_r2), .r3(M_r3),
        .data(M_out), .v1(v1_reg), .v2(v2_reg)
    );

    logic D_eq;
    logic [4:0] D_r1, D_r2, D_rd, D_sht;
    logic [31:0] D_imm, D_vs, D_vt;
    DecodeStage decode(
        .instr(instr0[25:0]), .pc(pc0),
        .signal(signal0[12:9]),
        .r1(D_r1), .r2(D_r2),
        .v1(v1), .v2(v2),
        .eq(D_eq), .rd(D_rd), .sht(D_sht),
        .imm(D_imm), .vs(D_vs), .vt(D_vt)
    );

    logic [4:0] E_rd;
    logic [31:0] E_out, E_vt;
    ExecuteStage execute(
        .rd(rd1), .sht(sht1),
        .imm(imm1), .vs(vs1), .vt(vt1),
        .signal(signal1[7:2]),
        ._rd(E_rd), .out(E_out), ._vt(E_vt)
    );

    logic M_data_src;
    assign M_data_src = signal2[0];
    assign mem_en = signal2[1];

    logic [4:0] M_r3;
    logic [31:0] M_out;
    MemoryStage memory(
        .rd(rd2), .out(out2), .vt(vt2),
        .data_src(M_data_src),
        .addr(addr), .mem_data(mem_data),
        .mem_out(mem_out),
        .r3(M_r3), .data(M_out)
    );

    logic [12:0] signal0;
    logic E_is_load;
    assign E_is_load = signal1[8];
    PipelineController pipeline(
        .opcode(instr0[31:26]), .funct(instr0[5:0]),
        .signal(signal0)
    );

    logic stall, v1_mux, v2_mux;
    logic [31:0] v1_fw, v2_fw;
    HazardController hazard(
        .E_is_load(E_is_load),
        .D_opcode(instr0[31:26]),
        .rs(D_r1), .rt(D_r2),
        .E_rd(rd1), .M_rd(rd2),
        .E_out(E_out), .M_out(M_out),
        .stall(stall),
        .v1_mux(v1_mux), .v2_mux(v2_mux),
        .v1_fw(v1_fw), .v2_fw(v2_fw)
    );

    assign v1 = v1_mux ? v1_fw : v1_reg;
    assign v2 = v2_mux ? v2_fw : v2_reg;

    always_ff @(posedge clk) begin
        if (reset) begin
            {pc0, instr0} <= 0;
            {signal1, rd1, imm1, vs1, vt1, sht1} <= 0;
            {signal2, rd2, out2, vt2} <= 0;
        end else begin
            rd2 <= E_rd;
            out2 <= E_out;
            vt2 <= E_vt;
            signal2 <= signal1[1:0];

            if (stall) begin
                {signal1, rd1, imm1, vs1, vt1, sht1} <= 0;
            end else begin
                rd1 <= D_rd;
                imm1 <= D_imm;
                vs1 <= D_vs;
                vt1 <= D_vt;
                sht1 <= D_sht;
                signal1 <= signal0[8:0];

                pc0 <= F_pc;
                instr0 <= F_instr;
            end
        end
    end
endmodule