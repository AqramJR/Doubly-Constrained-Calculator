Instructions:

• Users can use 'log.txt' to add a matrix in this form:
- Size of the matrix
- Matrix: one space between columns and one line for each row
- Sum of production with one space between two values
- Sum of attraction with one space between two values
  
• Alternatively, users can manually input the matrix into the program, and the program will save it in
'log.txt' for future use.

• If the sum of production and attraction is not equal, the program will ask if you want to correct it
and will print the corrected values.

• The program will prompt you for the acceptable error percentage; the less error, the more
iterations will be made. The maximum iteration is 1000.

• The output will include:
- Production Factors (a)
- Attraction Factors (b)
- Production Factor Error (a%)
- Attraction Factor Error (b%)
- Number of trials
- Factors Matrix (a * b)
- The Result Matrix.
  
• The program will ask the user to press 'E' to exit or 'R' to reload the program.


Example

Size of the matrix: 10

The matrix:

500 400 500 400 300 800 400 800 600 800

500 200 600 300 300 400 300 700 200 800

700 500 500 400 600 500 500 500 600 500

300 700 500 600 300 400 700 400 500 600

400 700 800 200 200 500 700 800 800 800

700 800 700 300 400 300 300 400 800 600

200 700 700 300 500 200 300 400 800 300

200 400 400 700 800 800 400 800 700 500

400 700 300 600 200 700 800 700 700 700

400 800 300 800 800 200 500 800 300 500

the sum of production: 
4200 6600 5300 8600 1700 2100 7500 9300 6000 7100

the sum of attraction: 
2800 3400 4100 2800 3300 3800 4600 2500 2600 4000

correction applied

Acceptable error: 0%


![image](https://github.com/AqramJR/Doubly-Constrained-Calculator/assets/99099093/6d321525-0872-4813-b0b2-43b82ae4df7a)

The result

Production Factors (a):

a1: 0.762296

a2: 1.50437

a3: 0.979886

a4: 1.6709

a5: 0.284465

a6: 0.3977

a7: 1.74487

a8: 1.66722

a9: 1.02521

a10: 1.35261


Attraction Factors (b):

b1: 1.10983

b2: 0.905195

b3: 1.23551

b4: 0.844076

b5: 1.03768

b6: 1.23461

b7: 1.4618

b8: 0.61182

b9: 0.698353

b10: 1.05639

Production growth factor Error (a%): 0%

Attraction growth factor error (b%): 0%

Number of trails: 14

Factors Matrix (a*b)


The result Matrix:

423.01    276.011    470.913    257.374    237.307    752.909    445.729    373.11    319.411    644.225

834.797    272.349    1115.2    380.94    468.317    742.921    659.724    644.281    210.116    1271.36

761.256    443.494    605.33    330.839    610.086    604.887    716.197    299.757    410.584    517.57

556.325    1058.74    1032.21    846.219    520.159    825.161    1709.76    408.915    583.438    1059.07

126.283    180.247    281.167    48.0219    59.0368    175.601    291.081    139.233    158.925    240.404

308.966    287.997    343.954    100.707    165.075    147.301    174.407    97.3284    222.188    252.076

387.303    1105.61    1509.07    441.841    905.311    430.846    765.194    427.019    974.828    552.979

370.067    603.664    823.947    985.082    1384.04    1646.69    974.855    816.03    815.015    880.617

455.124    649.61    379.997    519.213    212.768    886.012    1198.92    439.07    501.171    758.115

600.466    979.498    501.348    913.362    1122.86    333.987    988.618    662.041    283.379    714.44


![image](https://github.com/AqramJR/Doubly-Constrained-Calculator/assets/99099093/048cd8c7-d125-470d-b297-8fa0510ab4d2)






