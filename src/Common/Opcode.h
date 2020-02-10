#pragma once

#include <string>

// 操作码 虚拟机指令集
enum class Opcode
{
	nop, // 无操作
	halt, // 停止
	iconst, // 整型常量
	dconst, // 浮点型常量
	load, // 加载局部变量
	gload, // 加载全局变量
	store, // 存储局部变量
	gstore, // 存储全局变量
	add, // 加法
	sub, // 减法
	mul, // 乘法
	div, // 除法
	rem, // 取余
	equ, // 相等
	neq, // 不相等
	g, // 大于
	l, // 小于
	ge, // 大于等于
	le, // 小于等于
	neg, // 负
	jmp, // 无条件跳转
	jl, // 小于0时跳转
	jle, // 小于等于0时跳转
	jg, // 大于0时跳转
	jge, // 大于等于0时跳转
	je, // 等于0时跳转
	jne, // 不等于0时跳转
	call, // 函数调用
	ret, // 函数返回
	toi, // 强制转换成整数
	tod, // 强制转换成浮点数
	pop, // 弹出栈顶操作数
	land, // 逻辑与
	lor, // 逻辑或
	lnot, // 逻辑非
};

std::string GetOpcodeSymbol(Opcode op); // 获取指令文本