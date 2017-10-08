#include "stdafx.h"
#include "../SE-Sudoku/Sudoku.h"
#include "../SE-Sudoku/InputHandler.h"
#include "../SE-Sudoku/OutputHandler.h"
#include "../SE-Sudoku/Output.h"
#include "../SE-Sudoku/Sudoku.cpp"
#include "../SE-Sudoku/InputHandler.cpp"
#include "../SE-Sudoku/OutputHandler.cpp"
#include "../SE-Sudoku/Output.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		Sudoku sudoku;
		const int length = 100;
		int argc;
		char** argv;

		UnitTest1() {
			/*
				@overview:Initialize unittest
			*/
			argv = new char*[6];
			for (int i = 0; i < 6; i++)
			{
				argv[i] = new char[100];
			}
		}

		TEST_METHOD(TestSolve)
		{
			/*
			@overview:test the method Sudoku::solve
			*/
			int puzzle[81] = {
				5,1,2,3,4,0,7,8,9,
				7,8,9,5,1,2,3,4,6,
				3,4,6,7,8,9,5,1,2,
				2,5,1,0,3,4,9,6,7,
				6,9,7,2,5,1,8,3,4,
				8,3,4,6,9,7,2,5,0,
				1,2,5,4,7,3,6,9,8,
				9,6,8,0,2,5,4,7,3,
				4,7,3,9,6,8,1,2,5 };
			int solution[81] = {
				5,1,2,3,4,6,7,8,9,
				7,8,9,5,1,2,3,4,6,
				3,4,6,7,8,9,5,1,2,
				2,5,1,8,3,4,9,6,7,
				6,9,7,2,5,1,8,3,4,
				8,3,4,6,9,7,2,5,1,
				1,2,5,4,7,3,6,9,8,
				9,6,8,1,2,5,4,7,3,
				4,7,3,9,6,8,1,2,5
			};
			int temp[81];
			bool ret;
			ret = sudoku.solve(puzzle, temp);
			Assert::AreEqual(ret, true);
			for (int i = 0; i < 81; ++i) {
				Assert::AreEqual(temp[i], solution[i]);
			}
		}
		TEST_METHOD(TestGenerate1) {
			/*
			@overview:test the method Sudoku::void Sudoku::generate
			(int number, int lower, int upper, bool unique, int result[][LEN*LEN])
			*/
			int result[10][81];
			int solution[81];
			int lower;
			int upper;
			//generate easy sudoku
			lower = 35;
			upper = 45;
			sudoku.generate(10, lower, upper, false, result);
			for (int i = 0; i < 10; ++i) {
				Assert::AreEqual(sudoku.solve(result[i], solution), true);
				int count = 0;
				for (int j = 0; j < 81; ++j) {
					if (result[i][j] == 0) count++;
				}
				Assert::AreEqual(count <= upper && count >= lower, true);
			}

		}
		TEST_METHOD(TestGenerate1_2) {
			//generate medium sudoku
			int lower = 45;
			int upper = 55;
			int number = 10;
			int result[10][81];
			int solution[81];
			sudoku.generate(10, lower, upper, true, result);
			for (int i = 0; i < number; ++i) {
				Assert::AreEqual(sudoku.solve(result[i], solution), true);
				int solutionNumber = sudoku.countSolutionNumber(result[i], 2);
				Assert::AreEqual(solutionNumber, 1);
				int count = 0;
				for (int j = 0; j < 81; ++j) {
					if (result[i][j] == 0) count++;
				}
				Assert::AreEqual(count <= upper && count >= lower, true);
			}
		}
		TEST_METHOD(TestGenerate2) {
			int result[10][81];
			sudoku.generate(10, EASYMODE, result);
			for (int i = 0; i < 10; ++i) {
				int count = 0;
				for (int j = 0; j < 81; ++j) {
					if (result[i][j] == 0) count++;
				}
				Assert::AreEqual(count <= EASYUPPER && count >= EASYLOWER, true);
			}

			sudoku.generate(10, NORMALMODE, result);
			for (int i = 0; i < 10; ++i) {
				int count = 0;
				for (int j = 0; j < 81; ++j) {
					if (result[i][j] == 0) count++;
				}
				Assert::AreEqual(count <= NORMALUPPER && count >= NORMALLOWER, true);
			}
			
			sudoku.generate(10, HARDMODE, result);
			for (int i = 0; i < 10; ++i) {
				int count = 0;
				for (int j = 0; j < 81; ++j) {
					if (result[i][j] == 0) count++;
				}
				Assert::AreEqual(count <= HARDUPPER && count >= HARDLOWER, true);
			}
			
		}
		TEST_METHOD(TestException) {
			int result[1][81];
			bool exceptionThrown = false;
			try { // Test first SudokuCountException
				sudoku.generate(-1, 1, result);
			}
			catch (SudokuCountException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);

			//test ModeRangeException
			exceptionThrown = false;

			try {
				sudoku.generate(1, -1, result);
			}
			catch (ModeRangeException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);

			//test ResultRowTooFewerException
			/*exceptionThrown = false;
			try {
				sudoku.generate(10, 1, result);
			}
			catch (ResultRowTooFewException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);*/

			//test generate2
			//test ResultRowTooFewerException
			/*exceptionThrown = false;
			try {
				sudoku.generate(10, 20, 50, true, result);
			}
			catch (ResultRowTooFewException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);*/

			//test SudokuCountRangeException
			exceptionThrown = false;
			try {
				sudoku.generate(100000, 20, 50, true, result);
			}
			catch (SudokuCountException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);

			//test LowerUpperException,case 1
			exceptionThrown = false;
			try {
				sudoku.generate(1, 1, 50, true, result);
			}
			catch (LowerUpperException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);
			//test LowerUpperException,case 2
			exceptionThrown = false;
			try {
				sudoku.generate(1, 20, 56, true, result);
			}
			catch (LowerUpperException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);
			//test LowerUpperException,case 3
			exceptionThrown = false;
			try {
				sudoku.generate(1, 50, 1, true, result);
			}
			catch (LowerUpperException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);

			//test solve,case1
			/*int puzzle[1];
			int solution[81];
			exceptionThrown = false;
			try {
				sudoku.solve(puzzle, solution);
			}
			catch (IllegalLengthException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);
			//test solve,case2
			int puzzle2[81];
			int solution2[1];
			exceptionThrown = false;
			try {
				sudoku.solve(puzzle, solution);
			}
			catch (IllegalLengthException& e) {
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);*/
		}
		TEST_METHOD(TestInputHandlerAnalyze1) {
			/*
				@overview:test inputhandler.analyze(),and its getter
			*/
			InputHandler* input;
			strcpy_s(argv[1], length, "-n");
			strcpy_s(argv[2], length, "1000");
			strcpy_s(argv[3], length, "-r");
			strcpy_s(argv[4], length, "20~55");
			strcpy_s(argv[5], length, "-u");
			argc = 6;
			input = new InputHandler(argc, argv);
			input->analyze();
			Assert::AreEqual(input->getMode(), 'n');
			Assert::AreEqual(input->getNumber(), 1000);
			Assert::AreEqual(input->getUpper(), 55);
			Assert::AreEqual(input->getLower(), 20);
			Assert::AreEqual(input->getUnique(), true);

			delete input;
		}
		TEST_METHOD(TestInputHandlerAnalyze2) {
			/*
			@overview:test inputhandler.analyze(),and its getter
			*/
			InputHandler* input;
			strcpy_s(argv[1], length, "-s");
			strcpy_s(argv[2], length, "puzzle.txt");
			argc = 3;
			input = new InputHandler(argc, argv);
			input->analyze();
			Assert::AreEqual(input->getMode(), 's');
			Assert::AreEqual(input->getFileName(), "puzzle.txt");
			
			delete input;
		}
		TEST_METHOD(TestInputHandlerAnalyze3) {
			/*
			@overview:test inputhandler.analyze(),and its getter
			*/
			InputHandler* input;
			strcpy_s(argv[1], length, "-c");
			strcpy_s(argv[2], length, "1000");
			argc = 3;
			input = new InputHandler(argc, argv);
			input->analyze();
			Assert::AreEqual(input->getMode(), 'c');
			Assert::AreEqual(input->getNumber(), 1000);

			delete input;
		}
		TEST_METHOD(TestGenerateCompleteNAndOutput) {
			/*
				@overview:test method sudoku.generateCompleteNAndOutput
			*/
			char* filename = "sudoku.txt";
			int board[LEN*LEN];
			int count=0;
			sudoku.generateCompleteNAndOutput(1000, filename);
			fstream f(filename, ios::in);
			InputHandler input(argc,argv);

			//should have 1000 valid sudoku
			while (input.getBoard(f, board)) {
				count++;
				sudoku.convertToTwoDimension(board);
				Assert::AreEqual(sudoku.check(), true);
			}
			Assert::AreEqual(count, 1000);
			f.close();
		}
		TEST_METHOD(Testconvert) {
			/*
				@overview:test sudoku.convertToOneDimension()
				and sudoku.convertToTwoDimension()
			*/
			int solution[81] = {
				5,1,2,3,4,6,7,8,9,
				7,8,9,5,1,2,3,4,6,
				3,4,6,7,8,9,5,1,2,
				2,5,1,8,3,4,9,6,7,
				6,9,7,2,5,1,8,3,4,
				8,3,4,6,9,7,2,5,1,
				1,2,5,4,7,3,6,9,8,
				9,6,8,1,2,5,4,7,3,
				4,7,3,9,6,8,1,2,5
			};
			sudoku.convertToTwoDimension(solution);
			for (int i = 0; i < 81; ++i) {
				Assert::AreEqual(sudoku.getElem(i / 9 + 1, i % 9 + 1), char(solution[i] + '0'));
			}
			int solution2[81];
			sudoku.convertToOneDimension(solution2);
			for (int i = 0; i < 81; ++i) {
				Assert::AreEqual(solution[i],solution2[i]);
			}
		}
		TEST_METHOD(TestCheck) {
			/*
			@overview:test sudoku.check()
			*/
			int solution1[81] = {
				5,1,2,3,4,6,7,8,9,
				7,8,9,5,1,2,3,4,6,
				3,4,6,7,8,9,5,1,2,
				2,5,1,8,3,4,9,6,7,
				6,9,7,2,5,1,8,3,4,
				8,3,4,6,9,7,2,5,1,
				1,2,5,4,7,3,6,9,8,
				9,6,8,1,2,5,4,7,3,
				4,7,3,9,6,8,1,2,5
			};
			int solution2[81] = {
				5,1,2,3,4,6,7,8,9,
				7,8,9,5,1,2,3,4,6,
				3,4,6,7,8,9,5,1,2,
				2,5,1,8,3,4,9,6,7,
				6,9,7,2,5,1,8,3,4,
				8,3,4,6,9,7,2,5,1,
				1,2,5,4,7,3,7,9,8,
				9,6,8,1,2,5,4,7,3,
				4,7,3,9,6,8,1,2,5
			};
			sudoku.convertToTwoDimension(solution1);
			Assert::IsTrue(sudoku.check());
			sudoku.convertToTwoDimension(solution2);
			Assert::IsFalse(sudoku.check());
		}
	};
}