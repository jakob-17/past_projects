package assignment7;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

/**
 * Class containing the checkFile method for checking if the (, [, and { symbols
 * in an input file are correctly matched.
 * 
 * @author Jakob Horvath, Sabrina Walton
 */
public class BalancedSymbolChecker {

	/**
	 * Returns a message indicating whether the input file has unmatched symbols.
	 * (Use the methods below for constructing messages.) Throws
	 * FileNotFoundException if the file does not exist.
	 * 
	 * @param filename -- file to check for unmatched symbols.
	 */
	@SuppressWarnings("resource")
	public static String checkFile(String filename) throws FileNotFoundException {

		MyStack<Character> stack = new MyStack<Character>();

		FileReader input = new FileReader(filename);

		Scanner s;
		char currentChar;
		int colNum;
		int rowNum = 0;
		String line;
		boolean blockCom = false; // Keeps track of if code is currently in a block comment.
		boolean stringLit = false; // Keeps track if code is currently in a String literal.

		s = new Scanner(input);
		while (s.hasNextLine()) {
			line = s.nextLine();
			rowNum++; // Keeps track of row number
			colNum = 0; // Reset column number.
			for (int j = 0; j < line.length(); j++) {
				currentChar = line.charAt(j);
				colNum++; // Keeps track of column number.

				// Looks for comment block beginning.
				if (currentChar == '/' && j + 1 < line.length() && line.charAt(j + 1) == '*')
					blockCom = true;

				// Looks for comment block ending.
				if (currentChar == '*' && j + 1 < line.length() && line.charAt(j + 1) == '/')
					blockCom = false;

				// Looks for String literals if not inside comment block. Assumes they are
				// always closed.
				if (currentChar == '"' && !blockCom) {
					if (!(line.charAt(j - 1) == '\\'))
						stringLit = !stringLit;
				}

				// Looks for char literals if not inside comment block. Assumes they are always
				// closed.
				if (currentChar == '\'' && !blockCom) {
					// If char literal needs '\' to make it legal, looks for that and skips over it,
					// char, and apostrophe.
					if (line.charAt(j + 1) == '\\')
						j += 3;
					else // Jumps over char and closing apostrophe.
						j += 2;
				}

				// if not currently in block comment or String literal
				if (!blockCom && !stringLit) {
					// if comment line, skip rest of line and continue
					if (currentChar == '/' && j + 1 < line.length() && line.charAt(j + 1) == '/')
						break;

					// if opening brace, add to top of the stack
					else if (currentChar == '{' | currentChar == '(' | currentChar == '[') {
						stack.push(currentChar);

						// if one of the three closing braces, check that the top of the stack contains
						// its match

					} else if (currentChar == '}') {
						if (stack.isEmpty())
							return unmatchedSymbol(rowNum, colNum, currentChar, ' ');
						if (stack.peek() == '(')
							return unmatchedSymbol(rowNum, colNum, currentChar, ')');
						if (stack.peek() == '[')
							return unmatchedSymbol(rowNum, colNum, currentChar, ']');
						stack.pop();

					} else if (currentChar == ']') {
						if (stack.isEmpty())
							return unmatchedSymbol(rowNum + 1, colNum, currentChar, ' ');
						if (stack.peek() == '(')
							return unmatchedSymbol(rowNum, colNum, currentChar, ')');
						if (stack.peek() == '{')
							return unmatchedSymbol(rowNum, colNum, currentChar, '}');
						stack.pop();

					} else if (currentChar == ')') {
						if (stack.isEmpty())
							return unmatchedSymbol(rowNum + 1, colNum, currentChar, ' ');
						if (stack.peek() == '[')
							return unmatchedSymbol(rowNum, colNum, currentChar, ']');
						if (stack.peek() == '{')
							return unmatchedSymbol(rowNum, colNum, currentChar, '}');
						stack.pop();
					}
				}
			}
		}

		// Checks that comment block was closed.
		if (blockCom == true) {
			return unfinishedComment();
		}

		// Checks stack remainder
		if (!stack.isEmpty()) {
			for (int k = 0; k < stack.size(); k++) {
				if (stack.peek() == '{')
					return unmatchedSymbolAtEOF('}');
				if (stack.peek() == '(')
					return unmatchedSymbolAtEOF(')');
				if (stack.peek() == '[')
					return unmatchedSymbolAtEOF(']');
			}
		}

		// all symbols match
		return allSymbolsMatch();
	}

	/**
	 * Returns an error message for unmatched symbol at the input line and column
	 * numbers. Indicates the symbol match that was expected and the symbol that was
	 * read.
	 */
	private static String unmatchedSymbol(int lineNumber, int colNumber, char symbolRead, char symbolExpected) {
		return "ERROR: Unmatched symbol at line " + lineNumber + " and column " + colNumber + ". Expected "
				+ symbolExpected + ", but read " + symbolRead + " instead.";
	}

	/**
	 * Returns an error message for unmatched symbol at the end of file. Indicates
	 * the symbol match that was expected.
	 */
	private static String unmatchedSymbolAtEOF(char symbolExpected) {
		return "ERROR: Unmatched symbol at the end of file. Expected " + symbolExpected + ".";
	}

	/**
	 * Returns an error message for a file that ends with an open /* comment.
	 */
	private static String unfinishedComment() {
		return "ERROR: File ended before closing comment.";
	}

	/**
	 * Returns a message for a file in which all symbols match.
	 */
	private static String allSymbolsMatch() {
		return "No errors found. All symbols match.";
	}
}
