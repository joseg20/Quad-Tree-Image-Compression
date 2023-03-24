
# QuadTree-Image-Compression

Quad-Tree image compression algorithm implemented in C using stb_image.h and stb_image_write.h libraries. Efficiently reduces image size while maintaining quality.


## Screenshots
Original Image
![Original Image](https://www.dropbox.com/s/56y0k5zkr0po5fv/valp.jpg?raw=1)
\
Out Image(Quadtree's Level = 11) 371.549 bytes
![Out Image L11](https://www.dropbox.com/s/9kc89a76qjh8mhm/out_level%20_11.jpg?raw=1)
\
Out Image(Quadtree's Level = 9) 266.116 bytes
![Out Image L9](https://www.dropbox.com/s/7xni7cys4ts7k6g/out.jpg?raw=1)

## Run Locally

Clone the project

```bash
  git clone https://link-to-project
```

Go to the project directory

```bash
  cd my-project
```

Install gcc

```bash
  apt-get install gcc
```

Compile the source code

```bash
  gcc src/image_compression.c -lm -o image_compression.exe 
```

Run the code

```bash
  ./image_compression.exe img/my-img.exe required-level
```
## Usage/Examples

```bash
  ./image_compression.exe img/valp.jpg 11
  ./image_compression.exe img/coolcat.jpg 7
  ./image_compression.exe img/gato.jpg 3
```


## Related

Here are some related projects

[STB Image](https://github.com/nothings/stb)


## Authors

- [@joseg20](https://github.com/joseg20)


## License

[MIT](https://choosealicense.com/licenses/mit/)



