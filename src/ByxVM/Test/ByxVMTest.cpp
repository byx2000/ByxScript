#include "ByxVMTest.h"
#include "../../Common/TestUtil.h"
#include "../ByxVM.h"

#include <iostream>
#include <cassert>

using namespace std;

void ByxVMTest::Run()
{
	/*StopWatch watch;
	watch.begin();

	// 整数存取
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::inc);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 301);
		ASSERT(ops.popInt() == 200);
	}

	// 浮点数存取
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.5);
		codeSeg.add(Opcode::dconst, 2.2);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 3.7);
		codeSeg.add(Opcode::dconst, 4.6);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(2, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popDouble() == 2.2);
		ASSERT(ops.popDouble() == 3.7);
		ASSERT(ops.popDouble() == 1.5);
	}

	// 整数和浮点数混合存取
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 37);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.2375);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::istore, 3);
		codeSeg.add(Opcode::iconst, 249);
		codeSeg.add(Opcode::istore, 4);
		codeSeg.add(Opcode::dload, 4);
		codeSeg.add(Opcode::dstore, 5);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::iload, 3);
		codeSeg.add(Opcode::dload, 5);
		Code code(codeSeg);

		FunctionTable table;
		table.add(6, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popDouble() == (double)249);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popDouble() == (double)37);
		ASSERT(ops.popInt() == 3);
	}

	// 整数四则运算
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::imul);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::idiv);
		codeSeg.add(Opcode::iconst, 7);
		codeSeg.add(Opcode::isub);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 3);
	}

	// 浮点数四则运算
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 2.75);
		codeSeg.add(Opcode::dconst, 3.6);
		codeSeg.add(Opcode::dconst, 9.1);
		codeSeg.add(Opcode::dmul);
		codeSeg.add(Opcode::dadd);
		codeSeg.add(Opcode::dconst, 6.5);
		codeSeg.add(Opcode::ddiv);
		codeSeg.add(Opcode::dconst, 3.8);
		codeSeg.add(Opcode::dsub);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popDouble() == (2.75 + 3.6 * 9.1) / 6.5 - 3.8);
	}

	// 整数和浮点数混合四则运算
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::dconst, 2.3);
		codeSeg.add(Opcode::idiv);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::dconst, 2.3);
		codeSeg.add(Opcode::ddiv);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::idiv);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::ddiv);
		codeSeg.add(Opcode::dstore, 3);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::dload, 3);
		Code code(codeSeg);

		FunctionTable table;
		table.add(4, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popDouble() == 2.0 / 3.0);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popDouble() == 6 / 2.3);
		ASSERT(ops.popInt() == 6 / 2);
	}

	// 整数除零
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::idiv);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		
		ASSERT_EXCEPTION(vm.exec());
	}

	// 浮点数除零
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::dconst, 0.0);
		codeSeg.add(Opcode::ddiv);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);

		ASSERT_EXCEPTION(vm.exec());
	}

	// icmp 整数比较
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1);
		codeSeg.add(Opcode::iconst, 30);
		codeSeg.add(Opcode::icmp);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 25);
		codeSeg.add(Opcode::iconst, 25);
		codeSeg.add(Opcode::icmp);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::iconst, 8);
		codeSeg.add(Opcode::icmp);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == -1);
	}

	// dcmp 浮点数比较
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::dconst, 3.15);
		codeSeg.add(Opcode::dcmp);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 23.75);
		codeSeg.add(Opcode::dconst, 23.75);
		codeSeg.add(Opcode::dcmp);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 23423.435);
		codeSeg.add(Opcode::dconst, 4.68);
		codeSeg.add(Opcode::dcmp);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 0);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == -1);
	}

	// 整数与浮点数混合比较
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::icmp);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::dcmp);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::dcmp);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == -1);
		ASSERT(ops.popInt() == 0);
	}

	// ig
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 13);
		codeSeg.add(Opcode::iconst, 27);
		codeSeg.add(Opcode::ig);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::ig);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 79);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::ig);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
	}

	// ige
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 13);
		codeSeg.add(Opcode::iconst, 27);
		codeSeg.add(Opcode::ige);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::ige);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 79);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::ige);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 1);
	}

	// il
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 13);
		codeSeg.add(Opcode::iconst, 27);
		codeSeg.add(Opcode::il);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::il);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 79);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::il);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 0);
	}

	// ile
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 13);
		codeSeg.add(Opcode::iconst, 27);
		codeSeg.add(Opcode::ile);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::ile);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 79);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::ile);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
	}

	// ie
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 13);
		codeSeg.add(Opcode::iconst, 27);
		codeSeg.add(Opcode::ie);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::ie);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 79);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::ie);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
	}

	// ine
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 13);
		codeSeg.add(Opcode::iconst, 27);
		codeSeg.add(Opcode::ine);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::iconst, 50);
		codeSeg.add(Opcode::ine);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 79);
		codeSeg.add(Opcode::iconst, 6);
		codeSeg.add(Opcode::ine);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
	}

	// iand
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 2725);
		codeSeg.add(Opcode::iconst, 66358);
		codeSeg.add(Opcode::iand);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 548);
	}

	// ior
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 2725);
		codeSeg.add(Opcode::iconst, 66358);
		codeSeg.add(Opcode::ior);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 68535);
	}

	// inot
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 2725);
		codeSeg.add(Opcode::inot);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == -2726);
	}

	// dg
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.6);
		codeSeg.add(Opcode::dconst, 27.34);
		codeSeg.add(Opcode::dg);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dg);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.778);
		codeSeg.add(Opcode::dconst, 0.726);
		codeSeg.add(Opcode::dg);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
	}

	// dge
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.6);
		codeSeg.add(Opcode::dconst, 27.34);
		codeSeg.add(Opcode::dge);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dge);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.778);
		codeSeg.add(Opcode::dconst, 0.726);
		codeSeg.add(Opcode::dge);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 1);
	}

	// dl
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.6);
		codeSeg.add(Opcode::dconst, 27.34);
		codeSeg.add(Opcode::dl);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dl);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.778);
		codeSeg.add(Opcode::dconst, 0.726);
		codeSeg.add(Opcode::dl);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 0);
	}

	// dle
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.6);
		codeSeg.add(Opcode::dconst, 27.34);
		codeSeg.add(Opcode::dle);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dle);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.778);
		codeSeg.add(Opcode::dconst, 0.726);
		codeSeg.add(Opcode::dle);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
	}

	// de
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.6);
		codeSeg.add(Opcode::dconst, 27.34);
		codeSeg.add(Opcode::de);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::de);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.778);
		codeSeg.add(Opcode::dconst, 0.726);
		codeSeg.add(Opcode::de);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
	}

	// dne
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::dconst, 1.6);
		codeSeg.add(Opcode::dconst, 27.34);
		codeSeg.add(Opcode::dne);
		codeSeg.add(Opcode::dstore, 0);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dconst, 332.454);
		codeSeg.add(Opcode::dne);
		codeSeg.add(Opcode::dstore, 1);
		codeSeg.add(Opcode::dconst, 0.778);
		codeSeg.add(Opcode::dconst, 0.726);
		codeSeg.add(Opcode::dne);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::dload, 1);
		codeSeg.add(Opcode::dload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
	}

	// jmp
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 5);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(1, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 2);
	}

	// jl
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1); // 0
		codeSeg.add(Opcode::jl, 5);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 7);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 0); // 7
		codeSeg.add(Opcode::jl, 12);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::jmp, 14);
		codeSeg.add(Opcode::iconst, 400);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1); // 14
		codeSeg.add(Opcode::jl, 19);
		codeSeg.add(Opcode::iconst, 500);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::jmp, 21);
		codeSeg.add(Opcode::iconst, 600);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0); // 21
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 500);
		ASSERT(ops.popInt() == 300);
		ASSERT(ops.popInt() == 200);
	}

	// jle
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1); // 0
		codeSeg.add(Opcode::jle, 5);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 7);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 0); // 7
		codeSeg.add(Opcode::jle, 12);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::jmp, 14);
		codeSeg.add(Opcode::iconst, 400);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1); // 14
		codeSeg.add(Opcode::jle, 19);
		codeSeg.add(Opcode::iconst, 500);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::jmp, 21);
		codeSeg.add(Opcode::iconst, 600);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0); // 21
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 500);
		ASSERT(ops.popInt() == 400);
		ASSERT(ops.popInt() == 200);
	}

	// jg
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1); // 0
		codeSeg.add(Opcode::jg, 5);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 7);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 0); // 7
		codeSeg.add(Opcode::jg, 12);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::jmp, 14);
		codeSeg.add(Opcode::iconst, 400);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1); // 14
		codeSeg.add(Opcode::jg, 19);
		codeSeg.add(Opcode::iconst, 500);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::jmp, 21);
		codeSeg.add(Opcode::iconst, 600);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0); // 21
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 600);
		ASSERT(ops.popInt() == 300);
		ASSERT(ops.popInt() == 100);
	}

	// jge
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1); // 0
		codeSeg.add(Opcode::jge, 5);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 7);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 0); // 7
		codeSeg.add(Opcode::jge, 12);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::jmp, 14);
		codeSeg.add(Opcode::iconst, 400);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1); // 14
		codeSeg.add(Opcode::jge, 19);
		codeSeg.add(Opcode::iconst, 500);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::jmp, 21);
		codeSeg.add(Opcode::iconst, 600);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0); // 21
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 600);
		ASSERT(ops.popInt() == 400);
		ASSERT(ops.popInt() == 100);
	}

	// je
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1); // 0
		codeSeg.add(Opcode::je, 5);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 7);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 0); // 7
		codeSeg.add(Opcode::je, 12);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::jmp, 14);
		codeSeg.add(Opcode::iconst, 400);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1); // 14
		codeSeg.add(Opcode::je, 19);
		codeSeg.add(Opcode::iconst, 500);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::jmp, 21);
		codeSeg.add(Opcode::iconst, 600);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0); // 21
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 500);
		ASSERT(ops.popInt() == 400);
		ASSERT(ops.popInt() == 100);
	}

	// jne
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, -1); // 0
		codeSeg.add(Opcode::jne, 5);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::jmp, 7);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 0); // 7
		codeSeg.add(Opcode::jne, 12);
		codeSeg.add(Opcode::iconst, 300);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::jmp, 14);
		codeSeg.add(Opcode::iconst, 400);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1); // 14
		codeSeg.add(Opcode::jne, 19);
		codeSeg.add(Opcode::iconst, 500);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::jmp, 21);
		codeSeg.add(Opcode::iconst, 600);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iload, 0); // 21
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		Code code(codeSeg);

		FunctionTable table;
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 600);
		ASSERT(ops.popInt() == 300);
		ASSERT(ops.popInt() == 200);
	}

	// 循环测试
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::istore, 0); // s = 0
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::istore, 1); // i = 1
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::icmp); 
		codeSeg.add(Opcode::jg, 16); // i > 100 ?
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::istore, 0); // s = s + i
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::inc);
		codeSeg.add(Opcode::istore, 1); // i++
		codeSeg.add(Opcode::jmp, 4);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(2, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 5050);
	}

	// 函数调用1
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 25); // function main
		codeSeg.add(Opcode::call, 1); // foo(25)
		codeSeg.add(Opcode::ret);
		codeSeg.add(Opcode::istore, 0); // function foo
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::imul);
		codeSeg.add(Opcode::ret);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);
		table.add(1, 3);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 625);
	}

	// 函数调用2
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::istore, 0); // int sum(int n)
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::istore, 1); // int s = 0
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::istore, 2); // int i = 1
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::icmp);
		codeSeg.add(Opcode::jg, 17); // i > n?
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::istore, 1); // s += i
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::inc);
		codeSeg.add(Opcode::istore, 2); // i++
		codeSeg.add(Opcode::jmp, 5);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::ret); // return s
		codeSeg.add(Opcode::iconst, 100); // void main()
		codeSeg.add(Opcode::call, 1); // sum(100);
		codeSeg.add(Opcode::iconst, 2378);
		codeSeg.add(Opcode::call, 1); // sum(1000);
		codeSeg.add(Opcode::ret); // return
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 19);
		table.add(3, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 2828631);
		ASSERT(ops.popInt() == 5050);
	}

	// 函数调用3
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::istore, 1); // int pow(int a, int b)
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::istore, 2); // r = 1
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::istore, 3); // i = 1
		codeSeg.add(Opcode::iload, 3);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::ile); // i <= b?
		codeSeg.add(Opcode::je, 18);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::imul);
		codeSeg.add(Opcode::istore, 2); // r *= a
		codeSeg.add(Opcode::iload, 3);
		codeSeg.add(Opcode::inc);
		codeSeg.add(Opcode::istore, 3); // i++
		codeSeg.add(Opcode::jmp, 6);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::ret); // return r
		codeSeg.add(Opcode::iconst, 6); // void main()
		codeSeg.add(Opcode::iconst, 17);
		codeSeg.add(Opcode::call, 1); // pow(17, 6)
		codeSeg.add(Opcode::ret);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 20);
		table.add(4, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 24137569);
	}

	// 函数调用4
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::istore, 0); // int fac(int n)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::ie); // n == 0 ?
		codeSeg.add(Opcode::jne, 6);
		codeSeg.add(Opcode::jmp, 8);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ret); // return 1
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fac(n - 1)
		codeSeg.add(Opcode::imul);
		codeSeg.add(Opcode::ret); // return n * fac(n - 1)
		codeSeg.add(Opcode::iconst, 5); // void main()
		codeSeg.add(Opcode::call, 1); // fac(5)
		codeSeg.add(Opcode::iconst, 10);
		codeSeg.add(Opcode::call, 1); // fac(10)
		codeSeg.add(Opcode::iadd); // fac(5) + fac(10)
		codeSeg.add(Opcode::ret);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 15);
		table.add(1, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 3628920);
	}

	// 函数调用5
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 20); // void main()
		codeSeg.add(Opcode::call, 1); // fib(20)
		codeSeg.add(Opcode::iconst, 23);
		codeSeg.add(Opcode::call, 1); // fib(23)
		codeSeg.add(Opcode::iadd); // fib(20) + fib(23)
		codeSeg.add(Opcode::ret);
		codeSeg.add(Opcode::istore, 0); // int fib(int n)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ie); // n == 1 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::ie); // n == 2 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::jmp, 18);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ret); // return 1
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 1)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 2)
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::ret); // return fib(n - 1) + fib(n - 2)
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);
		table.add(1, 6);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 35422);
	}

	// 异常测试1
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 20); // void main()
		codeSeg.add(Opcode::call, 1); // fib(20)
		codeSeg.add(Opcode::iconst, 23);
		codeSeg.add(Opcode::call, 1); // fib(23)
		codeSeg.add(Opcode::iadd); // fib(20) + fib(23)
		codeSeg.add(Opcode::ret);
		codeSeg.add(Opcode::istore, 0); // int fib(int n)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ie); // n == 1 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::ie); // n == 2 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::jmp, 18);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ret); // return 1
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 1)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 2)
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::ret); // return fib(n - 1) + fib(n - 2)
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);
		table.add(0, 6);

		ByxVM vm(code, table, 0);
		ASSERT_EXCEPTION(vm.exec());
	}

	// 异常测试2
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 20); // void main()
		codeSeg.add(Opcode::call, 2); // fib(20)
		codeSeg.add(Opcode::iconst, 23);
		codeSeg.add(Opcode::call, 1); // fib(23)
		codeSeg.add(Opcode::iadd); // fib(20) + fib(23)
		codeSeg.add(Opcode::ret);
		codeSeg.add(Opcode::istore, 0); // int fib(int n)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ie); // n == 1 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::ie); // n == 2 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::jmp, 18);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ret); // return 1
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 1)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 2)
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::ret); // return fib(n - 1) + fib(n - 2)
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);
		table.add(1, 6);

		ByxVM vm(code, table, 0);
		ASSERT_EXCEPTION(vm.exec());
	}

	// 异常测试3
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 20); // void main()
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::call, 1); // fib(20)
		codeSeg.add(Opcode::iconst, 23);
		codeSeg.add(Opcode::call, 1); // fib(23)
		codeSeg.add(Opcode::iadd); // fib(20) + fib(23)
		codeSeg.add(Opcode::ret);
		codeSeg.add(Opcode::istore, 0); // int fib(int n)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ie); // n == 1 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::ie); // n == 2 ?
		codeSeg.add(Opcode::jne, 16);
		codeSeg.add(Opcode::jmp, 18);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::ret); // return 1
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 1)
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iconst, 2);
		codeSeg.add(Opcode::isub);
		codeSeg.add(Opcode::call, 1); // fib(n - 2)
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::ret); // return fib(n - 1) + fib(n - 2)
		Code code(codeSeg);

		FunctionTable table;
		table.add(1, 0);
		table.add(1, 6);

		ByxVM vm(code, table, 0);
		ASSERT_EXCEPTION(vm.exec());
	}

	// 全局变量测试
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::igstore, 0);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::dgstore, 1);
		codeSeg.add(Opcode::igload, 0);
		codeSeg.add(Opcode::iconst, 10);
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::igstore, 0);
		codeSeg.add(Opcode::dgload, 1);
		codeSeg.add(Opcode::dconst, 2.0);
		codeSeg.add(Opcode::dmul);
		codeSeg.add(Opcode::dgstore, 1);
		codeSeg.add(Opcode::igload, 0);
		codeSeg.add(Opcode::dgload, 1);
		codeSeg.add(Opcode::ret);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 2);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popDouble() == 3.14 * 2);
		ASSERT(ops.popInt() == 110);
	}

	// 异常测试4
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::igstore, 0);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::dgstore, 1);
		codeSeg.add(Opcode::igload, 0);
		codeSeg.add(Opcode::iconst, 10);
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::igstore, 0);
		codeSeg.add(Opcode::dgload, 1);
		codeSeg.add(Opcode::dconst, 2.0);
		codeSeg.add(Opcode::dmul);
		codeSeg.add(Opcode::dgstore, 1);
		codeSeg.add(Opcode::igload, 0);
		codeSeg.add(Opcode::dgload, 1);
		codeSeg.add(Opcode::ret);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		
		ASSERT_EXCEPTION(vm.exec());
	}

	// 异常测试5
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::igstore, 0);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::dgstore, 1);
		codeSeg.add(Opcode::igload, 0);
		codeSeg.add(Opcode::iconst, 10);
		codeSeg.add(Opcode::iadd);
		codeSeg.add(Opcode::igstore, 0);
		codeSeg.add(Opcode::dgload, 1);
		codeSeg.add(Opcode::dconst, 2.0);
		codeSeg.add(Opcode::dmul);
		codeSeg.add(Opcode::dgstore, 1);
		codeSeg.add(Opcode::igload, 0);
		codeSeg.add(Opcode::dgload, 1);
		codeSeg.add(Opcode::ret);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 1);

		ASSERT_EXCEPTION(vm.exec());
	}

	// toi tod
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::toi);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::toi);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::tod);
		codeSeg.add(Opcode::dstore, 2);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::tod);
		codeSeg.add(Opcode::dstore, 3);
		codeSeg.add(Opcode::dload, 3);
		codeSeg.add(Opcode::dload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(4, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 100);
		ASSERT(ops.popInt() == 3);
		ASSERT(ops.popDouble() == 100.0);
		ASSERT(ops.popDouble() == 3.14);
	}

	// pop
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 100);
		codeSeg.add(Opcode::iconst, 200);
		codeSeg.add(Opcode::dconst, 3.14);
		codeSeg.add(Opcode::pop);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 200);
		ASSERT(ops.popInt() == 100);
		ASSERT_EXCEPTION(ops.popInt());
	}

	// rem
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::iconst, 10);
		codeSeg.add(Opcode::rem);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 10);
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::rem);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iload, 0);
		codeSeg.add(Opcode::iload, 1);
		Code code(codeSeg);

		FunctionTable table;
		table.add(2, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 3);
	}

	// rem异常
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 3);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::rem);
		Code code(codeSeg);

		FunctionTable table;
		table.add(0, 0);

		ByxVM vm(code, table, 0);
		ASSERT_EXCEPTION(vm.exec());
	}

	// land
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::land);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::land);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::land);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::land);
		codeSeg.add(Opcode::istore, 3);
		codeSeg.add(Opcode::iload, 3);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(4, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
	}

	// lor
	{
		CodeSeg codeSeg;
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::lor);
		codeSeg.add(Opcode::istore, 0);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::lor);
		codeSeg.add(Opcode::istore, 1);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.add(Opcode::lor);
		codeSeg.add(Opcode::istore, 2);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.add(Opcode::lor);
		codeSeg.add(Opcode::istore, 3);
		codeSeg.add(Opcode::iload, 3);
		codeSeg.add(Opcode::iload, 2);
		codeSeg.add(Opcode::iload, 1);
		codeSeg.add(Opcode::iload, 0);
		Code code(codeSeg);

		FunctionTable table;
		table.add(4, 0);

		ByxVM vm(code, table, 0);
		vm.exec();
		OperandStack ops = vm.getOpStack();

		ASSERT(ops.popInt() == 0);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 1);
		ASSERT(ops.popInt() == 1);
	}

	watch.end();
	cout << "ByxVM test passed. time: " << watch.duration() << "s" << endl;*/
}