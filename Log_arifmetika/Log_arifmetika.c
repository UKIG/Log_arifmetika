#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS_IN_CEIL 7
#define MAX_STR_SIZE 6000
#define long_num_digit long long
#define long_num long_num_digit*
#define max(a,b) a > b ? a : b


int ident1_1(int val) {
	return 2 * val - 1;
}


int numberOfDigits(long long val) {
	int ans = 0;
	while (val) {
		val /= 10;
		ans++;
	}
	return ans;
}



int isDigit(char c) {
	return '0' <= c && c <= '9';
}



int getDigitCount(long_num longNumber) {
	return longNumber[0];
}


int getSign(long_num longNumber) {
	return longNumber[1];
}


int getCeilCountByDigitCount(int digitCount) {
	return digitCount / DIGITS_IN_CEIL + (digitCount % DIGITS_IN_CEIL != 0) + 2;
}


int getCeilCount(long_num longNumber) {
	return getCeilCountByDigitCount(getDigitCount(longNumber));
}

long_num_digit getCeilByIndex(long_num longNumber, int i) {
	int ceilCount = getCeilCount(longNumber);
	if (i < ceilCount)
		return longNumber[i];
	return 0;
}


long_num_digit getMod() {
	long_num_digit power = 1;
	for (int i = 0; i < DIGITS_IN_CEIL; i++)
		power *= 10;
	return power;
}



long_num initLongNumberBase(int digitCount, int sign) {
	int ceilCount = getCeilCountByDigitCount(digitCount);
	long_num newLongNumber = (long_num)malloc(ceilCount * sizeof(long_num_digit));
	if (newLongNumber) {
		newLongNumber[0] = digitCount;
		newLongNumber[1] = sign;
	}
	return newLongNumber;
}


long_num initLongNumberByStr(char* str) {
	int minusCounter = 0;
	int start = 0;
	while (str[start] && str[start] == '-')
		start++, minusCounter++;
	while (str[start] && str[start] == '0')
		start++;

	int end = start;
	while (str[end] && isDigit(str[end]))
		end++;

	long_num longNumber = initLongNumberBase(end - start, (end - start != 0) * minusCounter % 2);
	int ceilIndex = 2;
	const long_num_digit mod = getMod();
	start--;
	end--;
	while (end > start) {
		int power = 1,
			temp = 0;
		while (end > start && power < mod) {
			temp += power * (long_num_digit)(str[end] - '0');
			power *= 10;
			end--;
		}
		longNumber[ceilIndex++] = temp;
	}
	return longNumber;
}


long_num initLongNumberByInt(int val) {
	int digitCount = numberOfDigits(val);
	long_num newLongNumber = initLongNumberBase(digitCount, (val < 0));
	int ceilCount = getCeilCount(newLongNumber);
	long_num_digit mod = getMod();
	if (val < 0)
		val *= -1;
	for (int i = 2; i < ceilCount; i++) {
		newLongNumber[i] = val % mod;
		val /= mod;
	}
	return newLongNumber;
}



long_num updateLongNumber(long_num val) {
	int ceilCount = getCeilCount(val);
	while (ceilCount > 1 && val[ceilCount - 1] == 0)
		ceilCount--;
	if (ceilCount == 1) {
		free(val);
		return initLongNumberByInt(0);
	}
	int digitCounter = (ceilCount - 3) * DIGITS_IN_CEIL + numberOfDigits(val[ceilCount - 1]);
	long_num longNumber = initLongNumberBase(digitCounter, getSign(val));
	for (int i = 1; i < ceilCount; i++)
		longNumber[i] = val[i];
	free(val);
	return longNumber;
}


int lowEqualGreat(long_num lval, long_num rval) {
	if (getSign(lval) != getSign(rval))
		return ident1_1(getSign(lval) < getSign(rval));

	int signAccent = ident1_1(1 - getSign(lval));
	if (getDigitCount(lval) != getDigitCount(rval))
		return signAccent * ident1_1(getDigitCount(lval) > getDigitCount(rval));

	int ceilIndex = getCeilCount(lval) - 1;
	while (ceilIndex > 1) {
		if (lval[ceilIndex] != rval[ceilIndex])
			return signAccent * ident1_1(lval[ceilIndex] > rval[ceilIndex]);
		ceilIndex--;
	}
	return 0;
}


long_num unMinus(long_num val) {
	long_num newLongNumber = initLongNumberBase(getDigitCount(val), 1 - getSign(val));
	int ceilCount = getCeilCount(val);
	for (int i = 2; i < ceilCount; i++)
		newLongNumber[i] = val[i];
	return newLongNumber;
}



long_num diff(long_num, long_num);


long_num sum(long_num lval, long_num rval) {
	if (getSign(lval) != getSign(rval))
		return diff(lval, unMinus(rval));
	const long_num_digit mod = getMod();
	const int ceilSaves = 2 * DIGITS_IN_CEIL;
	long_num tempLongNumber = initLongNumberBase(max(getDigitCount(lval), getDigitCount(rval)) + ceilSaves, getSign(lval));
	const int ceilCount = getCeilCount(tempLongNumber);
	long_num_digit surplus = 0;
	for (int i = 2; i < ceilCount; i++) {
		surplus += getCeilByIndex(lval, i) + getCeilByIndex(rval, i);
		tempLongNumber[i] = surplus % mod;
		surplus /= mod;
	}
	return updateLongNumber(tempLongNumber);
}



long_num diff(long_num lval, long_num rval) {
	if (getSign(lval) != getSign(rval))
		return sum(lval, unMinus(rval));
	const int abscond = ident1_1(1 - getSign(lval)) * lowEqualGreat(lval, rval);
	if (abscond == 0)
		return initLongNumberByInt(0);

	long_num max_ = rval;
	long_num min_ = lval;
	if (abscond == 1)
		max_ = lval, min_ = rval;

	const long_num_digit mod = getMod();
	long_num tempLongNumber = initLongNumberBase(getDigitCount(max_), (abscond + 1) / 2 == getSign(lval));
	for (int i = getCeilCount(max_) - 1; i > 1; i--) {
		tempLongNumber[i] = getCeilByIndex(max_, i) - getCeilByIndex(min_, i);
		for (int j = i; tempLongNumber[j] < 0; j++) {
			tempLongNumber[j] += mod;
			tempLongNumber[j + 1] -= 1;
		}
	}
	return updateLongNumber(tempLongNumber);
}




long_num multiply(long_num lval, long_num rval) {
	const long_num_digit mod = getMod();
	const int ceilSaves = 4 * DIGITS_IN_CEIL;
	long_num tempLongNumber = initLongNumberBase(getDigitCount(lval) + getDigitCount(rval) + ceilSaves, (getSign(lval) + getSign(rval)) % 2);
	const int ceilCount = getCeilCount(tempLongNumber);
	long_num_digit surplus = 0;
	for (int i = 2; i < ceilCount; i++) {
		for (int j = 2; j <= i; j++)
			surplus += getCeilByIndex(lval, j) * getCeilByIndex(rval, i - j + 2);
		tempLongNumber[i] = surplus % mod;
		surplus /= mod;
	}
	return updateLongNumber(tempLongNumber);
}



long_num divBy2(long_num val) {
	const int saveCeils = 4 * DIGITS_IN_CEIL;
	const int digitCount = getDigitCount(val) + saveCeils;
	long_num tempLongNumber = initLongNumberBase(digitCount, getSign(val));
	long_num temp = initLongNumberByInt(-1);
	if (lowEqualGreat(val, temp) == 0) {
		free(temp);
		return initLongNumberByInt(0);
	}

	const int ceilCount = getCeilCount(tempLongNumber);
	const long_num_digit specMod = getMod() / 2;
	tempLongNumber[2] = getCeilByIndex(val, 2) / 2;
	for (int i = 3; i < ceilCount; i++) {
		tempLongNumber[i] = getCeilByIndex(val, i) / 2;
		if (getCeilByIndex(val, i) % 2)
			tempLongNumber[i - 1] += specMod;
	}
	return updateLongNumber(tempLongNumber);
}




long_num power(long_num lval, long_num rval) {
	long_num zero = initLongNumberByInt(0);
	if (lowEqualGreat(rval, zero) == 0) {
		free(zero);
		return initLongNumberByInt(1);
	}
	long_num newrval = divBy2(rval);
	long_num newlval = power(lval, newrval);
	free(newrval);
	newlval = multiply(newlval, newlval);
	if (rval[2] % 2)
		newlval = multiply(newlval, lval);
	free(zero);
	return newlval;
}




long_num range_sum(long_num lval, long_num rval) {
	long_num stack[5];
	stack[0] = initLongNumberByInt(1);
	stack[1] = diff(rval, lval);
	stack[2] = sum(stack[0], stack[1]);
	stack[3] = sum(lval, rval);
	stack[4] = multiply(stack[2], stack[3]);
	long_num ans = divBy2(stack[4]);
	for (int i = 0; i < 5; i++)
		free(stack[i]);
	return ans;
}




long_num factorial(long_num val) {
	long_num zero = initLongNumberByInt(0);
	if (lowEqualGreat(val, zero) == 0) {
		free(zero);
		return initLongNumberByInt(1);
	}
	int n = val[2];
	long_num ans = initLongNumberByInt(1);
	for (int i = 2; i <= n; i++) {
		long_num temp1 = initLongNumberByInt(i);
		long_num temp2 = multiply(ans, temp1);
		free(ans);
		ans = temp2;
		free(temp1);
	}
	free(zero);
	return ans;
}




void longNumberOut(long_num longNumber) {
	if (getSign(longNumber))
		printf("-");
	int ceilCount = getCeilCount(longNumber);
	printf("%lld", longNumber[--ceilCount]);
	for (int i = ceilCount - 1; i > 1; i--)
		printf("%07lld", longNumber[i]);
}



int getLvl(char op) {
	if (op == '+' || op == '-')
		return 1;
	if (op == '*' || op == ' ')
		return 2;
	if (op == '^')
		return 3;
	if (op == '!')
		return 4;

}


void getPolishStack(char* str, char*** polishStack, int* stackSize) {
	*stackSize = 1;
	for (int i = 1; str[i]; i++) {
		if ((str[i] == '*' || str[i] == ' ' || str[i] == '+' || str[i] == '^' || str[i] == '-') && (isDigit(str[i - 1])))
			*stackSize += 2;
		if (str[i] == '!')
			*stackSize += 1;
	}
	*polishStack = (char**)malloc(*stackSize * sizeof(char*));
	char* opstack = (char*)malloc(*stackSize * sizeof(char));
	int up = 0;

	int start = 0;
	int end = 0;
	while (str[end] && str[end] == '-')
		end++;
	while (str[end] && str[end] == '0')
		end++;
	while (str[end] && isDigit(str[end]))
		end++;
	(*polishStack)[0] = (char*)malloc((end - start + 1) * sizeof(char));
	for (int j = start; j < end; j++)
		(*polishStack)[0][j - start] = str[j];
	(*polishStack)[0][end - start] = '\0';
	start = end;
	int i;
	for (i = 1; i < *stackSize;) {
		while (str[start] && str[start] == '!') {
			while (up && getLvl(opstack[up - 1]) >= getLvl(str[start])) {
				(*polishStack)[i] = (char*)malloc(2 * sizeof(char));
				(*polishStack)[i][0] = opstack[--up];
				(*polishStack)[i++][1] = '\0';
			}
			opstack[up++] = str[start++];
		}
		while (up && getLvl(opstack[up - 1]) >= getLvl(str[start])) {
			(*polishStack)[i] = (char*)malloc(2 * sizeof(char));
			(*polishStack)[i][0] = opstack[--up];
			(*polishStack)[i++][1] = '\0';
		}
		opstack[up++] = str[start++];

		if (i < *stackSize) {
			end = start;
			while (str[end] && str[end] == '-')
				end++;
			while (str[end] && str[end] == '0')
				end++;
			while (str[end] && isDigit(str[end]))
				end++;
			(*polishStack)[i] = (char*)malloc((end - start + 1) * sizeof(char));
			for (int j = start; j < end; j++)
				(*polishStack)[i][j - start] = str[j];
			(*polishStack)[i][end - start] = '\0';
			start = end;
			i++;
		}
	}
	while (up) {
		(*polishStack)[i] = (char*)malloc(2 * sizeof(char));
		(*polishStack)[i][0] = opstack[--up];
		(*polishStack)[i++][1] = '\0';
	}
}

long_num computerByStack(char** stack, int* stackSize) {
	*stackSize -= 1;
	char* val = stack[*stackSize];
	if (strlen(val) > 1 || isDigit(val[0]))
		return initLongNumberByStr(val);
	long_num rval = computerByStack(stack, stackSize);
	if (strcmp(val, "!") == 0)
		return factorial(rval);
	long_num lval = computerByStack(stack, stackSize);

	if (strcmp(val, "+") == 0)
		return sum(lval, rval);
	if (strcmp(val, "-") == 0)
		return diff(lval, rval);
	if (strcmp(val, "*") == 0)
		return multiply(lval, rval);
	if (strcmp(val, "^") == 0)
		return power(lval, rval);
	if (strcmp(val, " ") == 0)
		return range_sum(lval, rval);
}

long_num computer(char* str) {
	int stackSize;
	char** stack;
	getPolishStack(str, &stack, &stackSize);
	return computerByStack(stack, &stackSize);
}

char* delete(char* a) {
	char* buf = (char*)malloc(strlen(a) * sizeof(char));
	int strLen = strlen(a) + 1;
	strcpy(buf, a);
	for (int i = 0; i < strlen(a); i++)
	{
		if ((buf[i] == ' ') && (buf[i + 1] == '+' || buf[i + 1] == '-' || buf[i + 1] == '^' || buf[i + 1] == '*' || buf[i + 1] == '!')) {
			memcpy(buf + i, buf + i + 1, strLen - i + 1);
			i--;
		}
		else if ((buf[i] == ' ') && (buf[i - 1] == '+' || buf[i - 1] == '-' || buf[i - 1] == '^' || buf[i - 1] == '*' || buf[i - 1] == '!')) {
			memcpy(buf + i, buf + i + 1, strLen - i + 1);
			i--;
		}
	}

	free(a);
	return buf;
}
int removing_the_sign_before_the_digitt(char* a) {
	int count = 0;
	for (int i = 0; i < strlen(a); i++) {
		if ((a[i] == '-' && (a[i + 1] >= '0' && a[i + 1] <= '9')) || (a[i] >= '0' && a[i] <= '9')) {
			break;
		}
		else {
			count++;
		}
	}
	return count;
}

int main() {
	char* a = (char*)malloc(MAX_STR_SIZE * sizeof(char));
	int s = 0;
	char* p;

	if (a) {
		fgets(a, MAX_STR_SIZE, stdin);

		a[strcspn(a, "\n")] = 0;
		for (int i = 0; a[i] != 0; i++) {
			if ((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z'))
				return 0;
		}
		s = removing_the_sign_before_the_digitt(a);
		memmove(a, a + s, strlen(a));

		longNumberOut(computer(delete(a)));




	}
	return 0;
}
