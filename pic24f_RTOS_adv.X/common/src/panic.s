/*
 * pic24f RTOS based OS
 * Copyright (C) 2015 <spiriou31@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

.include "p24FJ64GB002.inc"

.section .bss
.global _wRegSave, _TrapAddr, _ErrorAddrHigh, _ErrorAddrLow

_wRegSave: .space 32
_TrapAddr: .space 2
_ErrorAddrHigh: .space 2
_ErrorAddrLow: .space 2

.section .text

.global __OscillatorFail 
.weak __OscillatorFail
__OscillatorFail:
rcall __TrapError

.global __AddressError 
.weak __AddressError
__AddressError:
rcall __TrapError

.global __StackError 
.weak __StackError
__StackError:
rcall __TrapError

.global __MathError 
.weak __MathError
__MathError:
rcall __TrapError

.global __TrapError 
.weak __TrapError
__TrapError:
mov w0, _wRegSave
mov #_wRegSave + 2, w0
mov w1, [w0++]
mov w2, [w0++]
mov w3, [w0++]
mov w4, [w0++]
mov w5, [w0++]
mov w6, [w0++]
mov w7, [w0++]
mov w8, [w0++]
mov w9, [w0++]
mov w10, [w0++]
mov w11, [w0++]
mov w12, [w0++]
mov w13, [w0++]
mov w14, [w0++]
mov w15, [w0++]
pop w2 ; Get Trap Addr High -- toss it
pop w2 ; Get Trap Addr Low
mov #handle(__OscillatorFail), w1 ; get offset added to trap addr
sub #2, w1 ; (stupid compiler/linker)
sub w2, w1, [w0++] ; compute original trap addr
pop w2 ; Get Error Addr High
and w2, #7, [w0++] ; just the address
pop [w0++] ; Get Error Addr Low

call _panic_handler ; call our C routine to report the problem

.global __Trapped
/*
* In case OsError_Report returns -- just loop
*/
__Trapped:
bra __Trapped
