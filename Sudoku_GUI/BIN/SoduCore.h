// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SODUCORE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SODUCORE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SODUCORE_EXPORTS
#define SODUCORE_API __declspec(dllexport)
#else
#define SODUCORE_API __declspec(dllimport)
#endif

SODUCORE_API void generate_m(int number, int mode, int **result);
SODUCORE_API void generate_r(int number, int lower, int upper, bool unique, int **result);
SODUCORE_API bool solve_s(int *puzzle, int *solution);

SODUCORE_API bool dlxFind(int N, bool random, void ResDealing(const int *Res));