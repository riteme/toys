int popcount(int x) {
    int m1 = (0x55 << 8) | 0x55;
    int m2 = (0x33 << 8) | 0x33;
    int m3 = (0xF << 8) | 0xF;
    int m4 = (0xFF << 16) | 0xFF;
    int m5 = (0xFF << 8) | 0xFF;
    int mask = ~(1 << 31);
    int v = x & mask;
    m1 |= (m1 << 16);
    m2 |= (m2 << 16);
    m3 |= (m3 << 16);
    v = (v & m1) + ((v >> 1) & m1);
    v = (v & m2) + ((v >> 2) & m2);
    v = v + (v >> 4);
    v &= m3;
    v = v + (v >> 8);
    v &= m4;
    v = v + (v >> 16);
    return (v & m5) + !(!(x >> 31));
}