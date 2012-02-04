# My Macros

Macro making seems to be a dying or lost art form/practice. I hope to inspire some to take advantage of one of the original "syntax candy" paradigms.

These are just a few of the macros I have created to make code easier to read and implement.

## Macro Tips

- Macros are named based on how you want them to be used or perceived.
	- Use all caps if it starts a block and you have a separate macro that ends the block.
	- Use a function-like name if you want it to be used like a function or if you want it to look like a function (having little impact when expanded)
	
- Macros should never have more than three parameters. If possible, keep it two or less, but three max.
	- It comes down to being consistent, you want your macro to be a shortcut, easier to remember and execute for the programmer. Adding several parameters now means the dev has a harder time committing the macro signature to memory. Two or less is the easiest.
	- The macro name should, as much as possible, imply the parameter(s) it expects (if any). It's much harder to do when you have several parameters.
	
- Macros should try as much as possible to limit the impact of local variables. Meaning, don't add any local variables that are not expected/wanted by the dev.
	- A macro should avoid creating a global/local variable. It should use one's that are expected to be in scope.
        - Use curly braces to confine macro vars to a scope: `({ local_macro_var = 'here'; })`
	
- Don't abuse macroing. Meaning, don't turn an OOP langauge into a functional one by encapsulating to much functionality in macros.
	- Macros are not as descriptive as other OOP calls (example: Objective-C method signatures).
	- Macros are not very nice to real-time lexicon parsers. Some auto-complete engines have trouble with macros. Xcode expands them amazingly well, but barfs when using/typing macro parameters.
	- Don't use a macro, when a static function can do the same thing with the yeild the same results. Functions play nicer with IDEs and are "smarter".
