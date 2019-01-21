// pokedex replace hook
.org 0x0806F3A8
	ldr r3, =(add_exception_poketools|1)
	bx r3
.pool

.org 0x083A7344
.word sm_poketools_text
.pool

.org 0x083A7344 + 4
.word sm_poketools|1
.pool

.org 0x0806F404
.word sm_poketools|1
.pool
