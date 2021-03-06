# ma_jit (JIT binary translator)


## Shortly
**ma_jit is JIT (just-in-time) binary translator**, that translates **byte-code of [ma_proc](https://github.com/matmuher/flip-flop/tree/main/processor)** (my virtual machine) **to x86** instruction set.

## Translation

Original ma_proc instruction set consists of 27 instructions. You can check full list [here](processor/cmd.h). ma_jit processes major part of them. 

To simplify translation original instructions were divided into logic units. Translation by units:

1. Arithmetic
2. Memory access
3. Jumps
4. Functions
5. hlt

### 1. Arithmetic

In ma_proc arithmetic is based on stack. Step by step any operation can be represented as:

- two operands are pushed
- arithmetic operation is performed
- result is pushed 

Due to this algorithm translation looks like this:

<table>
	<tr>
		<th> ma_proc </th>
		<th> x86 </th>
	</tr>
	<tr>
		<td> add </td>
		<td>
			<br>pop rax <br>
			pop rcx <br>
			add rax, rcx <br>
			push rax <br> <br>
		</td>
	</tr>
	<tr>
		<td> sub </td>
		<td>
			<br>pop rax <br>
			pop rcx <br>
			sub rax, rcx <br> 
			push rax <br> <br>
		</td>
	</tr>
	<tr>
		<td> mlt </td>
		<td>
			<br>pop rax <br>
			pop rcx <br>
			cqo *<br>
			imul <br>
			push rax <br> <br>
		</td>
	</tr>
		<tr>
		<td> saw </td>
		<td>
			<br>pop rax <br>
			pop rcx <br>
			cqo <br>
			idiv <br> 
			push rax <br> <br>
		</td>
	</tr>
</table>

	 * due to peculiarities of x86 we need to extend rax to rdx sign-respectively

### 2. Memory access

ma_proc have 4 registers: AX, BX, CX, DX. In translated binary they correspond to R10, R11, R12, R13 respectively. 

ma_proc uses stack to carry out operations. x86 has stack as well, so memory access operations looks quite similar.

In ma_proc there is RAM memory. In x86 RAM memory is located after section with code. It's location is put in RSI and R11 initially. In ma_proc we can apply directly to RAM, in translted code it would interpreted as RSI relative addressing.
.
In ma_proc set BX is used in register-relative RAM addressing. In translated R11 is responsible for it.

In ma_proc we can address to RAM straightly. In x86 version 

<table>
	<tr>
		<th> ma_proc </th>
		<th> x86 </th>
	</tr>
	<tr>
		<td> push 1  </td>
		<td>
			push imm
		</td>
	</tr>
	<tr>
		<td> push [bx+1] </td>
		<td>
			push [r11 + 1]
		</td>
	</tr>
	<tr>
		<td> push ax <br>
			push bx<br>
			push cx<br>
			push dx<br>
		 </td>
		<td>push r10 <br>
			push r11<br>
			push r12<br>
			push r13<br>
		 </td>
		</td>
	</tr>
		<tr>
		<td> push [2] </td>
		<td>
			push [rsi+2]
		</td>
	</tr>
</table>

### 3. Jumps

To make a jump destination address is needed. In ma_proc this address is absolute offset from the start of code. In translated version relative jump is used.

Jump processing is implemented in two runs. In the first run array in which every ma_proc instruction corresponds to some address in JIT buffer (start of x86 equivalent instruction / instruction sequence)  is created.

In the second run these address are used to compute relative jump argument.

Unconditional jumps are implemented with use of native x86 conditional jumps and cmp operation. Operands as with arithmetic are poped in RAX and RCX.
### 4. Functions

ma_proc have 4 standard functions:

- `in`
- `out`
- `sqrt`
- `say`

In x86 translation of this functions is implemented as call of functions, that are wrappers over standard functions: 

<stdio.h>: `in` - `scanf`, `ou`t and `say` - `printf`

<math.h>: `sqrt` - `sqrt` 

According to System V ABI calling convention when calling a function stack should be 16-byte boundary aligned. There is push-pop counter in ma_jit that keeps track of whether stack is aligned. And if it is not PUSH_EMPTY (sub RSP, 8) and POP_EMPTY (add RSP, 8) are used.

### 5. hlt

`hlt` is instruction of finishing work for ma_proc. In translation it stands for putting return value in RAX and returning to bin_execute() function using ret address from the stack.

## Boost

Let's compare performance of executing program on virtual machine and with JIT translation. We will measure time with Linux 	`time` utility.

As a stress test I executed [fact.txt](asm_scripts/fact.tx). It computes factorial of 12 for 2,000,000 times. ma_jit's time includes warm_up time.

| ma_proc	|  ma_jit	|
|-----|---|
| (200 +- 5) * 10 ^ (-1)	|	(176 +- 3) * 10 ^ (-3) |

All in all, about 100 times boost!