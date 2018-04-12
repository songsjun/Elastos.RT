# The example of building android app(HelloElastosDemo)

## 1.Compile android-version RT

```
$ source SetEnv.sh arm_android
$ emake
```

## 2.Generate header files and libs files of elastos

* Execute the following order, you will find include files and lib files under directory of SDK/android/elastos.

```
dropsdk
```

## 3.Integrate elastos library information and third-party library information

* integrate elastos library

copy the contents gerenated in step 2 to Sources/Sample/HelloCarDemo/Android/HelloElastosDemo/app/src/main/jni/elastos/

```
Elastos.RT/SDK/android/elastos$cp -rf * ../../../Sources/Sample/HelloCarDemo/Android/HelloElastosDemo/app/src/main/jni/elastos/
```

* integrate third-party library 


copy the contents gerenated under Elastos.RT/Export/android/Elastos.HelloCarDemo to Sources/Sample/HelloCarDemo/Android/HelloElastosDemo/app/src/main/jni/Elastos.HelloCarDemo/


```
Elastos.RT/Export/android/Elastos.HelloCarDemo$ 
cp *  ../../../Sources/Sample/HelloCarDemo/Android/HelloElastosDemo/app/src/main/jni/Elastos.HelloCarDemo/
```