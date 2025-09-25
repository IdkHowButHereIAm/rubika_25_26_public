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

## Threshold 2

The purpose of this threshold is to implement an entity-component system to understand how common game engines (such as Unreal Engine or Unity) work. By creating this system, a animated sprite component and using the `TextureMgr` we have created during **Threshold 1**, we will create a animation system that can be use on every entity in our engine to play any animation which respects the right format.

### Step 0

Let's start with the entity/component part. An `Entity` is the base class of our system. Every thing is an entity (some entity will be special and will have access to more feature in the future) and those entities might have components that will add features, as we discussed in class.

To do so, a new file has been added `Entity.h` which provides the global architecture of our entities.
`IComponent.h` provides the base design of all our components. So, to create a new component, the new class must inherit from `IComponent` (take a look at the `TransformComponent` to see an implementation). Let's start with this component for now.

About the `Entity` part, you first need to implement functions about its own life cycle. The all component system is lacking, so you must add it so an entity can use `IComponent`.
```
/*
 * Constructor of the Entity class. The friendlyName parameter is for debugging purpose
 */
Entity(std::string friendlyName = "");

/*
 * Destructor of the Entity class. All allocated resources must be released
 */
virtual ~Entity();

/*
 * First function that will be called when an entity will be added to the current scene.
 */
virtual void Start();

/*
 * Function that will be called each frame if our entity is part of the current scene.
 */
virtual void Update(float fDeltaTime);

/*
 * Last function that will be called before an entity is removed of the current scene.
 */
virtual void Destroy();

/*
 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
  * Function that will be called each frame to draw the entity
 */
void Draw(sf::RenderWindow& window) const;
```

### Step 1

Our `Entity` is ready and can be integrated to the engine flow.
To do so, look at the `GameMgr.h`and `GameMgr.cpp` that have been added to the project and to the `Globals` class. 

**You must init this manager in the same way you did for the `TextureMgr`. The more you are coherent with yourself, the cleaer you code will be.**

An `Update` function and a `Draw` function have been added to the `Globals` class. Implement it and call them at the right moment in your flow.

We now have a working flow but it lacks of component. It is time to add them.

### Step 2

Looking back at `Entity.h`, there are still functions that have not been implemented, the intersting ones:
```
/*
* Add a new component of type C to the list of components.
* Be sure that we don't have the same component already registered.
* If an error occurs, return nullptr. Otherwise, creates the component and returns its pointer.
*/
template <typename C>
C* AddComponent();

/*
* Get an existing component of type C that has previously be added.
* If an error occurs or if there is no component of type C, returns nullptr.
* Otherwise, returns its pointer.
*/
template <typename C>
C* GetComponent() const;
```

**hints:**
- As we discussed in class, templated functions cannot be implemented in cpp. You have to define them in the same header as their declaration. A good way to do it, is to create `hxx` files that we be included at the end of your `.h` which defines those functions.
- To avoid having, awful errors in your console because of template, it is a good habit to use static checks with your template to ensure the given type will work with your implementation. For instance, in your case we want C to inherit from `IComponent`. This check can be made using `static_assert` and `std::is_base_of` (and the `requires` keyword in C++20).

### Step 3

Test your implementation by adding a `TranformComponent` to all entities by default.

### Step 4

Now, it is time to use every thing at once. Let's create a custom component `SpriteComponent` that will use a `sTextureData` to play an animation.
To do so, a `SpriteComponent.h` file has been added and contains the global architecture of this component (in the future, we will modify the inheritance tree to add a `IDrawable` layer between `IComponent` and `SpriteComponent`, so that lots of components can share the same code).

```
virtual void Start() override;

virtual void Update(float fDeltaTime) override;

virtual void Destroy() override;

virtual void Draw(sf::RenderWindow& window) const override;

void SetTexture(const std::string& textureName);
void SetAnimation(const std::string& animationName);

void PlayAnimation(bool bPause);
```

To play an animation contained in `sTextureData`, here are information about the internal structure:
- **X, Y** : Coordinates of the top-left corner of the animation in the texture
- **SizeX, SizeY**: Size of one frame of the animation
- **OffsetX, OffsetY** : Offset between two frames of an animation (distance between the right/botton side of the previous frame and the left/up side of the next frame)
- **SpriteNum** : Nunber of frame in the all animation
- **SpritesOnLine** : Number of frame on one line
- **Reverted**: Is the frame must be reverted (horizontal)
- **TimeBetweenAnimation**: Time between two frame

**Do not forget to use the `TransformComponent` to display the sprite at the right place**

At this state, you should be able to create an entity, assign an texture to it and play an animation.

## Threshold 3

Congratulation, you have created you first component (and your first system in a way).
The purpose of this Threshold is now to create some debug windows using `ImGui` to control them.

### Step 0

A new file `Debugs` has been added to the solution. As this name implies, this file must only compiled in `Debug` configuration and in `RelWithDebugInfo` configurarion. To do so, use MACRO (in the same way we used then in `Profiler.h`) to surrond the future code. At the same time, every call to `ImGui` must be removed in `Release` (move them behind a define) and `ImGui` files must be excluded from the solution in `Release` mode (so we won't waste time compiling then in this mode).

At the end :
- Compiling in `Debug` or `RelWithDebInfo` should compile `ImGui` files and enable the `Debugs` class
- Compiling in `Release` must not compile ImGUi or anything linked to the `Debugs` class

**hint :** It is much more clear to use a `_USE_IMGUI` define (that will be set or not depending of the configuration), so that by reading the name of the define, you will instantly know what is its purpose.

### Step 1

Now we have configure our solution to only compile what we really need, it is time to implent our `Debugs` class.
The purpose of this class is to gather all debug windows, commands... that would help us visualize our scene/project.

The console is now part of the `Debugs` class, everything has be changed to make it happen without breaking anything. Replace in you main function the call to `Logger::DrawLogger` by a call to `Debugs::DrawDebugWindow`.

You now have to implement the following function to draw a window and display in a tab the content of the Logger (you might want to reuse the Logger`s draw function but you can modify it if you want).

**Note : Less guidelines will be given for the next steps to make you think about your architecture and how to make things happen. Futhermore, UI (and UX) is important while creating debugs otherwise no one will ever use them. Try to think about that while implementing the next steps. For those reasons, only features (and small UI advices) will be given8**

### Step 2

Let's start with a simple visualization : create a debug to inspect the content of the `TextureMgr`. In a dedicated window, here are a list of feature I want you to implement :
- Being able to visualize all resources loaded in the `TextureMgr`. That include name/path, `sAnimationData`, sf::Texture information such as height, width...
- Add a ref count to `sTextureData` to determine how many `SpriteComponent` use this texture (you might need to change few things in your engine). Display which texture is used or not (by changing its color in the window for instance)

### Step 3

Let's continue with a debug panel : create a tab to inspect all entity in our scene : 
- Display the list of entities in our scene
- Being able to add a new entity, and to remove one
- Being able to inspect the components of an entity and modify their value
  * Being able to update data in the `TransformComponent` that will update the global display of the scene.
  * Being able to have information about which animation is played, which frame... And being able to change the texture/animation at runtime. To do so, it would be a good idea not to let the user writting things but to have a UI that will displayed the possible choice.
- Being able to add/remove a component from an entity.
