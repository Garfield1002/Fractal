# Mandelbrot fractal viewer

## 📚 Usage

I'm not too sure for the moment

## 🖼 Example results

## 🎮 Controls

| Key | Action |
| :-: | :----: |
| `⬆` | Move the camera up |
| `➡` | Move the camera right |
| `⬇` | Move the camera down |
| `⬅` | Move the camera left |
| `W` | Zoom in |
| `S` | Zoom out |

## 💡 hat I learned

### C++

This was one of my first C++ and OpenGL projects, so quite a lot. Here are a few takeaways:

- `uniforms` in GLSL to pass data to the shaders
- Classes and Header files (`Shader::Shader`)

### CMake

Their was an attempt at using CMake, maybe in a future project.

### Mandelbrot

I had to read through a couple wikipedia articles to learn about implementing the mandelbrot fractal.
I used their _"smoothing function"_: 

```glsl
	float log_zn = log(Z.x*Z.x + Z.y*Z.y) / 2.0;

	float nu = log(log_zn / log(2.0)) / log(2.0);

    iteration += 1.0 - nu;

    color = vec4(mix(colormap[int(floor(iteration)) % 17], colormap[(int(floor(iteration)) + 1) % 17], fract(iteration)), 1.0);
```
