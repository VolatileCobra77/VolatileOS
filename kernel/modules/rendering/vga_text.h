#include "../color.h"
#include <cstdint>
#include "../pointers.h"

struct Character {
    enum class Value : int {
        NUL = 0, SOH = 1, STX = 2, ETX = 3, EOT = 4, ENQ = 5, ACK = 6, BEL = 7,
        BS = 8, TAB = 9, LF = 10, VT = 11, FF = 12, CR = 13, SO = 14, SI = 15,
        DLE = 16, DC1 = 17, DC2 = 18, DC3 = 19, DC4 = 20, NAK = 21, SYN = 22, ETB = 23,
        CAN = 24, EM = 25, SUB = 26, ESC = 27, FS = 28, GS = 29, RS = 30, US = 31,
        SPACE = 32, EXCLAMATION = 33, QUOTE = 34, HASH = 35, DOLLAR = 36, PERCENT = 37,
        AMPERSAND = 38, SINGLE_QUOTE = 39, LEFT_PAREN = 40, RIGHT_PAREN = 41,
        ASTERISK = 42, PLUS = 43, COMMA = 44, MINUS = 45, DOT = 46, SLASH = 47,
        ZERO = 48, ONE = 49, TWO = 50, THREE = 51, FOUR = 52, FIVE = 53, SIX = 54, SEVEN = 55,
        EIGHT = 56, NINE = 57,
        COLON = 58, SEMICOLON = 59, LESS = 60, EQUAL = 61, GREATER = 62, QUESTION = 63, AT = 64,
        A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72, I = 73, J = 74, K = 75,
        L = 76, M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82, S = 83, T = 84, U = 85,
        V = 86, W = 87, X = 88, Y = 89, Z = 90,
        LEFT_BRACKET = 91, BACKSLASH = 92, RIGHT_BRACKET = 93, CARET = 94, UNDERSCORE = 95,
        BACKTICK = 96,
        a = 97, b = 98, c = 99, d = 100, e = 101, f = 102, g = 103, h = 104,
        i = 105, j = 106, k = 107, l = 108, m = 109, n = 110, o = 111, p = 112,
        q = 113, r = 114, s = 115, t = 116, u = 117, v = 118, w = 119, x = 120, y = 121, z = 122,
        LEFT_BRACE = 123, PIPE = 124, RIGHT_BRACE = 125, TILDE = 126, DEL = 127
    };

    Value value;

    // Constructor
    constexpr Character(Value v) : value(v) {}

    // Convert to char
    constexpr char toChar() const {
        return toChar(*this);
    }
    static constexpr char toChar(Character c){
        return static_cast<char>(c.value);
    }

    // Convert from char to Character, defaults to EXCLAMATION
    static constexpr Character fromChar(char c) {
        if (c >= 0 && c <= 127)
            return Character(static_cast<Value>(c));
        return Character(Value::EXCLAMATION);
    }
};

class Terminal {
public:
    int xSize;
    int ySize;
    volatile unsigned short KERNEL_VGA;
    Terminal(int xSize, int ySize, volatile unsigned short KERNEL_VGA) : xSize(xSize), ySize(ySize) , KERNEL_VGA(KERNEL_VGA){}

    void clear() {
        for (int i = 0; i < 10; i++) {
            printChar('C', TerminalColor(false, Color3B::BLACK, Color4B::BLACK), i);
        }
    }

    void printChar(char character, TerminalColor color, int location) {
        if (location < 0 || location >= xSize * ySize) location=0; // bounds check
        volatile unsigned short* vga = (volatile unsigned short*)KERNEL_VGA;
        volatile unsigned short cell = 0b0;
        cell |= (color.getColor() << 8);
        cell |= character;
        vga[location] = cell;
    }
};