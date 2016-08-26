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
$./assessImage.sh /path/to/retinal_image
```

## Sample Run
```
$ make all
$ ./assessImage.sh Images/1.jpg
```
## Output:
Ungradable Retinal Image<br>
<br><br>
<img src = "https://raw.githubusercontent.com/Kavitha-G/Retinal-Image-Quality-Assessment/master/SampleImages/IMG_00923.jpg" width = "40%" />
<br><br>
Gradable Retinal Image<br>
<br><br>
<img src = "https://raw.githubusercontent.com/Kavitha-G/Retinal-Image-Quality-Assessment/master/SampleImages/IMG_00924.jpg" width = "40%" />
<br><br>

  
