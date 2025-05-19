How many "buttons" have you clicked on the computer today? Probably a lot! From **Doom Dark Ages**, to **Discord**, every piece of software uses buttons to translate user input into the program.

But there are several unintuitive things when making a standard clickable button:

- it will not activate until the mouse button is pressed AND *released while the cursor is still hovering over the button*. Try it out with anything clickable on discord.
- It is inefficient to perform a cursor bounds-check on every button every frame. The **Engine** finds the currently hovered button *once per frame* and passes that via a mouse state object. The button can make decisions by comparing mouse state to its internal state.
- The button shouldn't have to know anything about what it is controlling. Therefore we give it pointers to the function it should call via @nimsolated [signal header](<https://github.com/nimsolated/Signal>).

Here is the [github]
