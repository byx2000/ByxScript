#pragma once

#include <string>

// ������ �����ָ�
enum class Opcode
{
	nop, // �޲���
	halt, // ֹͣ
	iconst, // ���ͳ���
	dconst, // �����ͳ���
	load, // ���ؾֲ�����
	gload, // ����ȫ�ֱ���
	store, // �洢�ֲ�����
	gstore, // �洢ȫ�ֱ���
	add, // �ӷ�
	sub, // ����
	mul, // �˷�
	div, // ����
	rem, // ȡ��
	equ, // ���
	neq, // �����
	g, // ����
	l, // С��
	ge, // ���ڵ���
	le, // С�ڵ���
	neg, // ��
	jmp, // ��������ת
	jl, // С��0ʱ��ת
	jle, // С�ڵ���0ʱ��ת
	jg, // ����0ʱ��ת
	jge, // ���ڵ���0ʱ��ת
	je, // ����0ʱ��ת
	jne, // ������0ʱ��ת
	call, // ��������
	ret, // ��������
	toi, // ǿ��ת��������
	tod, // ǿ��ת���ɸ�����
	pop, // ����ջ��������
	land, // �߼���
	lor, // �߼���
	lnot, // �߼���
};

std::string GetOpcodeSymbol(Opcode op); // ��ȡָ���ı�