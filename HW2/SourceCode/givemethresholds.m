clear;
p = 0.15
GradientThres = givemethreshold('GradientData_Grad.dat',p,'g')
LaplacianThres = givemethreshold('GradientData_Lapl.dat',p,'l')
DOGThres = givemethreshold('GradientData_Dog.dat',p,'d')