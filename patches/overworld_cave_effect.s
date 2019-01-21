// overworld effect animation for caves
// oe 0x1A script
.org 0x081D98A4
.byte 0x7
.word 0x083A5348
.word oei_1A_cave|1
.byte 0x4
.pool

// redundant
.org 0x081D96AC + 4 * 0x1A
.word 0x081D98A4


// extend animation frames for hidden water and cave
.org 0x083A5B56
.byte 0x8

.org 0x083A5B5A
.byte 0x8

.org 0x083A5B5E
.byte 0x8

.org 0x083A5B62
.byte 0x8

.org 0x083A5B66
.byte 0x8

.org 0x083A5B6A
.byte 0x8
