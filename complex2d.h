class Complex2D {
private:
    double real = 0;
    double im = 0;
public:
    Complex2D()=default;

    Complex2D(double new_real, double new_im);

    [[nodiscard]] double abs() const;

    Complex2D pow(int const &n);

    friend Complex2D operator+(Complex2D a, Complex2D b);

    friend Complex2D operator*(Complex2D a, Complex2D b);
};