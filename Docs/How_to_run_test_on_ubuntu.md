# Build Test cases

## HelloCar

1. enter the build environment(optional)
```
$ source ~/Elastos.RT/Setup/SetEnv.sh linux
```

2. build Elastos.HelloCar.eco
```
cd ~/Elastos.RT/Sources/Sample/HelloCar/eco
~/Elastos.RT/Sources/Sample/HelloCar/eco$ emake
```

3. build HelloCar
```
cd ~/Elastos.RT/Sources/Sample/HelloCar
~/Elastos.RT/Sources/Sample/HelloCar$ emake
```

4. build result
```
~/Elastos.RT/Sources/Sample/HelloCar$ pd @
~/Elastos.RT/Targets/rdk/x86.gnu.linux.dbg/bin$ ll HelloCar
```

5. run HelloCar
```
# Run the executable inside the dbg_info directory if you want to debug it with debug symbols
# We have already "export LD_LIBRARY_PATH=.". So, it will load the *.eco files inside the current directory
~/Elastos.RT/Targets/rdk/x86.gnu.linux.dbg/bin$ cd dbg_info
~/Elastos.RT/Targets/rdk/x86.gnu.linux.dbg/bin/dbg_info$ ./HelloCar
==================================
=========== Hello Car ============
==================================
Though cat has nine lives, but it can not fly!
Kitty can not fly!

FILE: Animal.cpp, LINE: 51, INFO: > Animal::constructor(2, HuaHua)

HuaHua : wang! wang!
HuaHua can not fly!

Though cat has nine lives, but it can not fly!
CAnimalHelper::CanFly : Kitty can not fly!

=========== Exit Hello Car ============
```

## Math

1. enter the build environment(optional)
```
$ source ~/Elastos.RT/Setup/SetEnv.sh linux
```

2. build Math test case
```
cd ~/Elastos.RT/Sources/Elastos/LibCore/tests/libcore/math
~/Elastos.RT/Sources/Elastos/LibCore/tests/libcore/math$ emake
```

3. build result
```
~/Elastos.RT/Sources/Elastos/LibCore/tests/libcore/math$ pd @
~/Elastos.RT/Targets/rdk/x86.gnu.linux.dbg/bin$ ll testMath
```

4. run testMath
```
# Run the executable inside the dbg_info directory if you want to debug it with debug symbols
# We have already "export LD_LIBRARY_PATH=.". So, it will load the *.eco files inside the current directory
~/Elastos.RT/Targets/rdk/x86.gnu.linux.dbg/bin$ cd dbg_info
~/Elastos.RT/Targets/rdk/x86.gnu.linux.dbg/bin/dbg_info$ ./testMath
```