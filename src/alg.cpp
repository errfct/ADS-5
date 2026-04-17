// Copyright 2025 NNTU-CS
#include <string>
#include <map>
#include "tstack.h"

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

int getPriority(char op) {
    if (op == '(') return 0;
    if (op == ')') return 1;
    if (op == '+' || op == '-') return 2;
    if (op == '*' || op == '/') return 3;
    return -1;
}

std::string infx2pstfx(const std::string& inf) {
  TStack<char, 100> stack;
  std::string result;

  for (size_t i = 0; i < inf.length(); ++i) {
    char current = inf[i];

    if (current == ' ') continue;

    if (isDigit(current)) {
      while (i < inf.length() && isDigit(inf[i])) {
        result += inf[i];
        ++i;
      }
      result += ' ';
      --i;
    } else if (current == '(') {
      stack.push(current);
    } else if (current == ')') {
      while (!stack.isEmpty() && stack.get() != '(') {
        result += stack.pop();
        result += ' ';
      }
      stack.pop();
    } else if (getPriority(current) > 1) {
      int currentPriority = getPriority(current);
      while (!stack.isEmpty() &&
          getPriority(stack.get()) >= currentPriority) {
        result += stack.pop();
        result += ' ';
      }
      stack.push(current);
    }
  }

  while (!stack.isEmpty()) {
    result += stack.pop();
    result += ' ';
  }

  if (!result.empty() && result.back() == ' ') {
    result.pop_back();
  }

  return result;
}

int eval(const std::string& pref) {
  TStack<int, 100> stack;

  size_t pos = 0;
  while (pos < pref.length()) {
    if (pref[pos] == ' ') {
      ++pos;
      continue;
    }

    if (isDigit(pref[pos])) {
      int num = 0;
      while (pos < pref.length() && isDigit(pref[pos])) {
        num = num * 10 + (pref[pos] - '0');
        ++pos;
      }
      stack.push(num);
    } else {
      if (stack.isEmpty()) {
        throw std::string("Invalid expression");
      }
      int b = stack.pop();
      if (stack.isEmpty()) {
        throw std::string("Invalid expression");
      }
      int a = stack.pop();

      switch (pref[pos]) {
        case '+':
          stack.push(a + b);
          break;
        case '-':
          stack.push(a - b);
          break;
        case '*':
          stack.push(a * b);
          break;
        case '/':
          if (b == 0) {
            throw std::string("Division by zero");
          }
          stack.push(a / b);
          break;
        default:
          throw std::string("Unknown operator");
      }
      ++pos;
    }
  }

  if (stack.isEmpty()) {
    throw std::string("Empty result");
  }
  return stack.pop();
}
