#include "string.h"
#include "stdlib.h"
#include "stdio.h"

/*
演算を行う場合に一番単純なのが逆ポーランド記法に置き換えてしまうことです。
普通の演算式を変換すると次のようになります。

1+2 → 1 2 +
1+3*4+5  →  1 3 4 * + 5 +
1+(2+3)*4 → 1 2 3 + 4 * +

とこんな感じです。

計算の仕方は演算子の場合に２個前と１個前の値を計算します。
例えば "1 2 3 + *" (1*(2+3))の式の計算手順は

1 2 3 + *     +の前の2と3を足して2の部分に計算結果を入れます
1 5 *         *の前の1と5を掛けて1の部分に計算結果を入れます
5             計算の答え

という感じで単純な計算ができます。

逆ポーランド記法というようにポーランド記法もあるわけですが，こっちは演算子が
先に来る記法だったと思いますが計算させようとする場合に逆ポーランド記法の方が
楽なので使いません。

ここまでが逆ポーランド記法の基礎です。

ソースコードは大きく分けて２つの部分で構成されています。
前半部分が普通の演算式を逆ポーランド記法に変換する部分，
後半が逆ポーランド記法を計算する部分です。

変換部分では+,-,/,*,(,)をサポートしてあります。
計算の都合上、渡された文字列の先頭に"("を追加して最後に")"を追加しています。

計算の部分では単純に計算しているだけです。
*/

///////////////////////////////////////////////////////////////////////////////
//ソースコード
//最長98文字で 0123456789.+-*/() で書いた数式を計算します。
double CalcString(char str[100])
{
	char varstack[100]; //演算子用のスタック
	char porstr[100][20]; //逆ポーランド記法配列（数値は文字列、他は1文字）
	char numstr[20]; //数値

	int i;
	int k = 0;//stacksize
	int varstacksize = 0;
	int numstrsize = 0;

	int strLen = strlen(str);
	if (strLen == 0)
	{
		printf("文字列が空です。\n");
		return 0.0;
	}
	if (strLen >= 98)
	{
		printf("文字列が長すぎます。\n");
		return 0.0;
	}

	//前後に '(', ')' を追加
	for (i = strLen; i > 0; i--)
		str[i] = str[i - 1];
	str[0] = '(';
	strLen += 2;
	str[strLen - 1] = ')';
	str[strLen] = '\0';

	for (i = 0; i < strLen; i++)
	{
		switch (str[i])
		{//入力文字列の先頭文字
		case '0': case '1': case '2': case '3': case '4': //数値の場合
		case '5': case '6': case '7': case '8': case '9': case '.':
			numstr[numstrsize] = str[i];//数値文字列に追加
			numstrsize++;
			break;
		case '+': //+,-の場合は演算子スタックを掃き出す
		case '-':
			if (numstrsize > 0)
			{
				numstr[numstrsize] = '\0';
				strcpy_s(porstr[k], numstr);
				k++;
				numstrsize = 0;
			}
			while (varstacksize > 0)
			{//演算子スタックが空でない場合
				if (varstack[varstacksize - 1] == '(')
				{//stack の top
					break;//トップが開き括弧のときはブレイク
				}
				//演算子スタックからポーランド記法へ
				porstr[k][0] = varstack[varstacksize - 1];
				porstr[k][1] = '\0';
				k++;
				varstacksize--;//演算子スタックから1文字削除
			}
			varstack[varstacksize] = str[i];
			varstacksize++;
			break;
		case '*':
		case '/': //*,/の場合は演算子スタックに追加
			if (numstrsize > 0)
			{
				//数値文字列があればポーランド記法に加える
				numstr[numstrsize] = '\0';
				strcpy_s(porstr[k], numstr);
				k++;
			}
			numstrsize = 0;//数値文字列の初期化（空にする）
			if (varstacksize > 0 && (varstack[varstacksize - 1] == '*' || varstack[varstacksize - 1] == '/'))
			{
				//演算子スタックからポーランド記法へ
				porstr[k][0] = varstack[varstacksize - 1];
				porstr[k][1] = '\0';
				k++;
				varstacksize--;//演算子スタックから1文字削除
			}
			varstack[varstacksize] = str[i];
			varstacksize++;
			break;
		case '(':
			varstack[varstacksize] = str[i];//演算子スタックへ
			varstacksize++;
			break;
		case ')':
			if (numstrsize > 0)
			{
				//数値文字列があればポーランド記法に加える
				numstr[numstrsize] = '\0';
				strcpy_s(porstr[k], numstr);
				k++;
			}
			numstrsize = 0;//数値文字列の初期化（空にする）
			while (varstacksize > 0)
			{//演算子スタックが空でない間
				if (varstack[varstacksize - 1] == '(')
				{
					varstacksize--;//開き括弧の時はスタックから消してブレイク
					break;
				}
				porstr[k][0] = varstack[varstacksize - 1];//ポーランド記法文字列へ
				porstr[k][1] = '\0';
				k++;
				varstacksize--;//演算子スタックから1文字削除
			}
			break;
		}
	}

	//  逆ポーランド記法の計算
	double numstack[100];
	int numstacksize = 0;

	for (i = 0; i < k; i++)
	{
		if (porstr[i][0] >= '0' && porstr[i][0] <= '9')
		{
			numstack[numstacksize] = atof(porstr[i]);
			numstacksize++;
		}
		else
		{
			if (numstacksize < 2)
			{
				printf("エラーです。\n");
				return 0.0;
			}
			double ope1, ope2;
			ope1 = numstack[numstacksize - 2];
			ope2 = numstack[numstacksize - 1];
			switch (porstr[i][0])
			{
			case '-': numstack[numstacksize - 2] = (ope1 - ope2);
				break;
			case '+': numstack[numstacksize - 2] = (ope1 + ope2);
				break;
			case '/': numstack[numstacksize - 2] = (ope1 / ope2);
				break;
			case '*': numstack[numstacksize - 2] = (ope1 * ope2);
				break;
			}
			numstacksize--;
		}
	}
	if (numstacksize != 1)
	{
		printf("エラーです。\n");
		return 0.0;
	}

	return numstack[0];
}

/// <summary>
/// Mains this instance.
/// </summary>
void main()
{
	char str[100];
	strcpy_s(str, "(1+2)*3");//入力数式
	printf("%f", CalcString(str));
	getchar();
}
