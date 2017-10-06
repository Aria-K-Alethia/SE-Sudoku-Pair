#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SE-Sudoku/Sudoku.h"
#include "../SE-Sudoku/Sudoku.cpp"
#include "../SE-Sudoku/InputHandler.cpp"
#include "../SE-Sudoku/InputHandler.h"
#include "../SE-Sudoku/Output.cpp"
#include "../SE-Sudoku/Output.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		Sudoku sudoku;
		
		UnitTest1() {
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
			ret = sudoku.solve(puzzle,temp);
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
			sudoku.generate(10, lower,upper, false, result);
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
			int lower = 10;
			int upper = 20;
			int number = 10;
			int result[10][81];
			int solution[81];
			sudoku.generate(10, lower, upper, true, result);
			for (int i = 0; i < number; ++i) {
				Assert::AreEqual(sudoku.solve(result[i], solution), true);
				int solutionNumber = sudoku.countSolutionNumber(result[i],2);
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
        TEST_METHOD(TestInputAnalyze){
            int result[1][81];
            bool exceptionThrown = false;
            try { // Test first SudokuCountException
                sudoku.generate(-1, 1, result);
            }
            catch (SudokuCountException& e) {
                exceptionThrown = true;
            }
            Assert::IsTrue(exceptionThrown);

            exceptionThrown = false;
        }
	};
}