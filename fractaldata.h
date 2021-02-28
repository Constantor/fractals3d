#pragma once

struct FractalData {
	double a{}, b{}, c{};
	unsigned int n{};

	FractalData() = default;
	FractalData(double a, double b, double c, unsigned int n) : a(a), b(b), c(c), n(n) {}
};