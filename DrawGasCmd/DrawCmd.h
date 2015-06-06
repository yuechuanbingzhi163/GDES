#pragma once

class DrawCmd
{
public:
	// 瓦斯图元(实现在DrawGas.cpp中)
	static void DrawGOP();
	static void DrawGCP();
	static void DrawGPP();
	static void DrawPPP();
	static void DrawDCI(int flag);
	static void DrawGFP(int flag);
	static void DrawCDH();

	static void DrawGOP2();
	static void DrawGCP2();
	static void DrawGPP2();
	static void DrawPPP2();
	static void DrawDCI2(int flag);
	static void DrawGFP2(int flag);
	static void DrawCDH2();

	static void DisplayDataByDoubleClick();
};

