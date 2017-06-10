class Complex():
    def __init__(self, real, imag):
        self.real = real 
        self.imag = imag

    def __add__(self, that):
        real = self.real + that.real
        imag = self.imag + that.imag
        return Complex(real, imag)

    def __sub__(self, that):
        real = self.real - that.real
        imag = self.imag - that.imag
        return Complex(real, imag)

c1 = Complex(1, 10)
c2 = Complex(2, 20)

c3 = c1 + c2
print c3.real
print c3.imag

c4 = c3 - c2
print c4.real
print c4.imag
