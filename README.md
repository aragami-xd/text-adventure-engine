a single header engine to build text-based decision adventure games

- [build and run](#build-and-run)
- [usage](#usage)
- [tl;dr version](#tldr-version)
- [storyline syntax](#storyline-syntax)
  - [tree](#tree)
  - [dialogs and decisions](#dialogs-and-decisions)
  - [id](#id)
  - [links](#links)
  - [game tree example](#game-tree-example)
- [more details to come](#more-details-to-come)

### build and run
include `engine.hpp` and compile with the project with c++17 and above (`std=c++17`)

<br>

### usage
- storyline of the game can be written in a text file and passed into the engine to be parsed into a working game tree
- extra configurations and functions can be configured later in your own game (details to come)

<br>

### tl;dr version
- trees contain dialogs, dialogs contain decisions
- dialogs start with `-`, decisions start with `+`, **MUST BE** the first character of the line, ortherwise it'll be normal text
- tree `id` will be the file name, start with the first dialog parsed in the file
- id: `$[id]`, tree links: `$T[]` or `$t[]`, dialog links: `$D[]` or `$d[]`. links jump to the id. can be put anywhere in the line
- if a dialog doesn't have a tree, it can only be reached by the dialog or decision above if they don't have any link
- if a decision doesn't have an id, the game will give it one
- if a decision doesn't have a link, or there is no decision at all, use dialog's link
- game ends when there is no further jumps can be made
  
<br>

### storyline syntax
#### tree
- a game tree, recommend create 1 tree per level and link with each other
- each file creates 1 tree
- the file name of the text file will be the [id](#id) of the tree
- the first dialog will be the "entry" point of the tree, whose message will be first displayed

<br>

#### dialogs and decisions
- mark with *starter* characters (`+` and `-`)
- use `-` for dialogs
- use `+` for decisions
- every decision after a dialog will be counted as that dialog's decisions, until the next dialog or end of file
- *WARNING*: these characters must be placed at the start (first character) of the line. otherwise, they'll be treated as normal text, part of the current dialog or decision
- if no starter is found, the line will be treated as normal text of the dialog / decision above
- all whitespace after the starter will be ignored up until the first character
- empty lines without starter will be dismissed. empty lines with starter can be kept (good for [linking](#links) elements around without showing any messages)

**decisions**:
- attached to the dialog above, can be chosen
- disabled decisions will either be displayed but cannot be chosen, or not displayed all together (config in the engine)

**examples**:
```
- this is a valid dialog
+ and this is a valid decision of the dialog above
+ and this is another valid decision of the same dialog

-        all whitespace before 'all' will be ignored, other whitespace like this    will be counted
 - this will not be counted as a dialog, just a part of the dialog above

-   +-+-+- (these +-+-+- symbols will be counted as normal text)
```

<br>

#### id
- id can be assigned to trees, dialogs and decisions for other dialogs and decisions to jump to using [links](#links)
- mark with *marker* sequence (`$[]`)
- use `$[id_name]` for ids, replacing `id_name` with the id specified. can be placed anywhere in the dialog / decision
- each dialog / decision can have 1 id only. subsequent ids will **cause exception**
- if no id is provided:
  - **tree**: program will throw an exception
  - **dialogs**: *WARNING*: this dialog will only reachable if there is a dialog above with no links (or its chosen decision has no link), in which case it'll jump to this dialog without any id needed
  - **decisions**: the engine will provide a unique id for it
- to escape the phrase, use 2 `$` characters: `$$[]` (can't use `\` because c doesn't like it). end result will be a single `$` character with the full marker

**examples**:
```
$[the first line of the file is the tree id] $D[dialog_id]

- this is a dialog id: $[dialog_id], and the tree links to this dialog as the first dialog
+ same with $[decisions]
+ the $[second] id will be $[ignored]: 'ignored' will be ignored and count as normal text of the sentence

- this $$[id] is escaped, therefore it does not count as an id
```

<br>

#### links
- links to other trees or dialogs
- like id, each decision / dialog contains a single link only. subsequent ones will **throw exception**
- use `$T[]` or `$t[]` for link to tree
- use `$D[]` or `$d[]` for link to dialog
- if there **is a** link:
  - **dialogs**: will prioritize decision link first (if it exists)
  - **decisions**, the decision will jump to this link after it's chosen
- if there **is no** link: 
  - **dialogs**: jump to the next dialog (that dialog don't need an id, although it can still have one). if there is no dialog after that, game ends
  - **decisions**: use dialog link, or the line above if dialog has no link either
  
*note*: the engine can be configured to trim all whitespace behind the markers up until the next piece of text
```
`text $[marker]     text` = text text
```

**examples**:
```
- $[dialog 1] $T[tree 1] this dialog links with 'tree 1'
+ $d[dialog 2] this decision links with `dialog 2`, and this $T[tree 2] will throw exception

- $[dialog 2] the decision above will jump to this dialog (ignore exception)
```

<br>

#### game tree example
tree 1 file: `tree1`
```
- $[da1] dialog 1, with no link
+ $[da1 dc1] decision 1, with link to dialog 2 $d[da2]
+ $[da1 dc2] decision 2, with link to dialog 3 $d[da3]
this one is still decision 2
+ $[da1 dc3] decision 3, with link to tree 2 $T[tree2]
+ $[da1 dc4] decision 4, with no link. therefore it uses dialog 1's link, which doesn't exist either, so it'll jump to dialog 2

- $[da2] 'da1 dc1' points to here, since this one has no link either, it'll jump to da3 below
- $[da3] pointed by 'da1 dc3' and 'da2'
```

tree 2 file: `tree2`
```
- $[da1] 'da1 dc3' points to this tree and this dialog here
```

### more details to come