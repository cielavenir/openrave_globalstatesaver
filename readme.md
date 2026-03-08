## openrave_globalstatesaver openrave(py) helper

- Provides `GlobalStateSaver` and `DataDirSaver` classes, which can be used to **override OPENRAVE_DATA without RaveDestroy, i.e. with keeping existing Environments.**

## Caveats

- Although I attached test cases, they might not be robust.
- DataDirSaver cannot be used after GlobalStateSaver. i.e. do not use both.
