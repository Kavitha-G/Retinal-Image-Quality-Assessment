# Retinal-Image-Quality-Assessment
Analyse adequacy of a retinal image

## Source
feature.cpp<br>
Reads in an image of the retina and calculates the feature vector values

result.cpp<br>
Takes in the feature vector and the coefficient vector(theta calculated using logistic regression) and declares whether the image is gradable or not

## Dependencies
* OpenCV 3.0.0

## Trial

Run: 
```
$ make all
$./AssessImage.sh /path/to/retinal_image
```

## Sample Run
```
$ make all
$ ./AssessImage.sh Images/1.jpg
```
## Output:
Ungradable Retinal Image<br>
<br>
<img src = "https://raw.githubusercontent.com/Kavitha-G/Retinal-Image-Quality-Assessment/master/SampleImages/IMG_00923.jpg" width = "40%" />
<br><br>
Gradable Retinal Image<br>
<br>
<img src = "https://raw.githubusercontent.com/Kavitha-G/Retinal-Image-Quality-Assessment/master/SampleImages/IMG_00924.jpg" width = "40%" />
<br><br>

## References
* Herbert Davis, Stephen Russell, Eduardo Barriga, Michael Abramoff and Peter Soliz - "Vision-based, Real-time Retinal Image Quality Assessment"
* Gopal Datt Joshi, Jayanthi Sivaswamy - "Colour Retinal Image Enhancement based on Domain Knowledge"
* A. Gebejes, R. Huertas - "Texture Characterization based on Grey-Level Co-occurrence Matrix"
* João Miguel Pires Dias, Carlos Manta Oliveira, Luís A. da Silva Cruz - "Retinal image quality assessment using generic image quality indicators"
