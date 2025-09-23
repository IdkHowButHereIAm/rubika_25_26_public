# MyEngine

The purpose of this project is to implement a tiny 2D engine using SFML to practice C++, to think about engine architecture, how to solve issues...

### Resources

Here are useful links:
- [SFML doc](https://www.sfml-dev.org/documentation/3.0.2/)
- [ImGui](https://pthom.github.io/imgui_manual_online/)

## Threshold 0

The purpose of this threshold is to (re)learn how to use SFML. You will have to draw sprites using resources that are provided (or the ones you like). Spawn several sprites which use the same texture.

## Threshold 1

The purpose of this threshold is to implement a `TextureMgr`, which, as its name implies, will be responsible of the management of all texures in our engine. From requesting them, to use them, which implies loading (the texture itself and its metadata) and storing.

### Step 0

A new file has been added `TextureMgr.h` and contains the base functions that must be implemented. But first, let's focus on the way we must instantiate this manager.
Another file has been added `Globals.h` which contains a unique entry point to the differents global variables we will need. This class must be accessible everywhere, so that any systems we will develop can use it. The way to do it is free, so you can use any method you want
**hint:** If you have no clue about how to do it, you might search for the *singleton* design pattern and/or remember about what we said on smart pointers

Once this part is done, don't forget to add this function in the life cycle of our engine.

### Step 1

Now our manager is instanciate and **unique**, it is time to implement it. First, let's focus on the texture part. We will deal with metadata later.

Two functions must be implemented :
```
	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& path);

    /*
    * Returns the corresponding TextureData using the name of the texture
    */
    const sTextureData& GetTextureData(const std::string& name) const;
```

Besides those two functions that are mandatory (if you want to change the way the system, please ask me first. So that the entire class has the same architecture), you can add any method/member/... you want/need. But keep in mind few things, which will be true for the rest of the project :
- A `Logger.h` file has been added. It provides a (small) working log system. Logs might be live saving and absolutely useless at the same time. They provide the user/developper with information about the various systems. But to be useful, logs must be added during the development and provide useful/self-explaining information.
- As you can see in `Profiler.h` (and used in `Main.cpp`), you can use macro to create markers that can be displayed in [PIX](https://devblogs.microsoft.com/pix/download/). This is a useful tool that can help you track costing process... But such as Logs, it is more simple to add markers during the development (otherwise, the moment you will really need it, it will be empty). 
- Try thinking about limit cases during all your development. By doing that, you will create robust system and limit the number of bugs you will create. For instance, in that case:
  * What will happen if the path doesn't exist
  * What will happen if there is no `sTextureData` that matches the texture name. Should it crash? Should we use a specific default texture?
  * ...

### Step 2

Now, the `TextureMgr` has been implemented, it is time to test it. Replace the all texture creation process (and its use) by the manager. If everything works as entended, you shouldn't see any difference with your previous code.

### Step 3

The texture itself has been loaded and can be used but, we need more information about what it contains, the animation... Those data can be provided in different way depending of the file format we use. In our case, let's assume that those data are provided by a `xml` file next to the image (same name but different extension). 

By looking at one of the provided `xml`, you will see that it provides a set of data about animations (coordinate of the rectangle that should be displayed, numer of frame, time...). The purpose of this step is to parse the file qnd store those data next to the texture (`IsaacSprite.xml` is the most complexe provided example).

**rapidxml** is part of the project and can be used by including 
```
// The main include
#include <rapidxml/rapidxml.hpp>

// Might be helpful
#include <rapidxml/rapidxml_utils.hpp>
```

Here are some hints:
- Before jumping to the implementation, take time to understand how a xml file is built. What is a node, an attribute, a parent, a child, a sibling, a value...
- Here is the [doc of rapidxml](https://rapidxml.sourceforge.net/manual.html).
- Use the content of `rapidxml/rapidxml_utils.hpp` to open the file (way more simple that using fopen)
- The lists of types that you should use:
  * rapidxml::file<> 
  * rapidxml::xml_document 
  * rapidxml::xml_node<> 
  * rapidxml::xml_attribute<>
- The list of functions that you should use:
  * parse<0> 
  * first_node 
  * first_attribute 
  * value 
  * value_size
  * next_sibling