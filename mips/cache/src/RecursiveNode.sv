module RecursiveNode #(
    LEFT = 1,
    RIGHT = 32,
    KEY_WIDTH = 4,
    _MID = (LEFT + RIGHT) / 2
) (
    input logic [31:0] tick[LEFT:RIGHT],
    output logic [KEY_WIDTH - 1:0] out
);
    if (LEFT == RIGHT) begin
        assign out = LEFT[KEY_WIDTH - 1:0];

        logic __unused_ok = &{1'b0, tick[LEFT], 1'b0};
    end else begin
        logic [KEY_WIDTH - 1:0] lout, rout;

        RecursiveNode #(
            .LEFT(LEFT),
            .RIGHT(_MID),
            .KEY_WIDTH(KEY_WIDTH)
        ) lch(tick[LEFT:_MID], lout);
        RecursiveNode #(
            .LEFT(_MID + 1),
            .RIGHT(RIGHT),
            .KEY_WIDTH(KEY_WIDTH)
        ) rch(tick[_MID + 1:RIGHT], rout);

        assign out = tick[lout] < tick[rout] ? lout : rout;
    end
endmodule