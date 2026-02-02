enum class Color3B : int{
    BLACK = 0b000,
    RED = 0b001,
    GREEN = 0b010,
    YELLOW = 0b011,
    BLUE = 0b100,
    MAGENTA = 0b101,
    CYAN = 0b110,
    WHITE = 0b111,
};
enum class Color4B : int{
    DARK_GRAY = 0b1000,
    RED = 0b1001,
    GREEN = 0b1010,
    YELLOW = 0b1011,
    BLUE = 0b1100,
    MAGENTA = 0b1101,
    CYAN = 0b1110,
    WHITE = 0b1111,
    BLACK = 0b0000,
    DARK_RED = 0b0001,
    DARK_GREEN = 0b0010,
    DARK_YELLOW = 0b0011,
    DARK_BLUE = 0b0100,
    DARK_MAGENTA = 0b0101,
    DARK_CYAN = 0b0110,
    LIGHT_GRAY = 0b0111,
};

class TerminalColor{
    public: Color4B foreground;
    public: Color3B background;
    public: bool blinking;


    TerminalColor(bool blinking, Color3B background, Color4B foreground){
        this->background = background;
        this->blinking = blinking;
        this->foreground = foreground;
    }

    int getColor(){
        int out = 0b0;
        out |= (blinking << 7);
        out |= ((static_cast<int>(background) & 0x7) << 4);
        out |= ((static_cast<int>(foreground) & 0xF) << 0);
        return out;
    }

};
