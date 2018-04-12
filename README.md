# ElastosRT [![Build Status](https://travis-ci.org/elastos/Elastos.RT.svg?branch=master)](https://travis-ci.org/elastos/Elastos.RT)

## Summary

ElastosRT is a development framework.

The programming idea with CAR is the essential technology in Elastos, it runs through the entire technology system implementation. In Elastos Runtime the component library provided by the Elastos framework is implemented using CAR technology. 

## Technology

### 1. CAR

- What is CAR?
  CAR means The Component Assembly Runtime (CAR). It is Java with machine code. It is COM with metadata.

  It is a component-oriented programming model and also a programming idea, and it is described by special standards divided in two parts: specification and implementation. The specification part tells how to define the components and how to communicate among components. Following the specification, any language can be used. Thus, CAR can be implemented in many ways. Elastos operating system implements the CAR core services.

  CAR components provide service using interface, component interface needs the metadata to describe the component so that other users can know how to use the service. Metadata describe the relationship between services and calls. With this description, calling between different components becomes possible, and members of the long-range, inter-process communication can be properly carried out. The major solved problems by CAR component technology are: component from different sources can interoperate; components upgrade but affect no other component; component is independent of the programming language; transparency of component operating environment.

- [CAR Language](Docs/CAR_Language.md)
- [Basic Data Type](Docs/CAR_Language.md#Data-Type)
- [Componet Class and Interface](Docs/CAR_Language.md#Keywords)


### 2. RPC by CAR interface

- [Carrier](Docs/Elastos_Carrier.md)
- [Service Manager](Docs/Service_Manager.md)


## Build

- [Getting Started](Docs/getting_started.md)
- [Build for android app](Docs/Example_HelloElastosDemo.md)
- [Build for CAR tools](Docs/How_to_build_CAR_tools_such_as_carc.md)
- [Build for test case](Docs/How_to_run_test_on_ubuntu.md)
- [Build Tips](Docs/build_tips.md)

## Development

- [CAR Programming](Docs/How_To_Write_A_Car_Componet.md)
- [Call a remote CAR componet (RPC)](Docs/How_To_Call_A_Remote_CAR_Componet.md)
- H5 App - JS call CAR (Coming soon ...)
- [Android App - Java call CAR](Sources/Sample/HelloCarDemo/Android/HelloElastosDemo/README.md)
- [iOS App - Object C call CAR](Sources/Sample/HelloCarDemo/iOS/app/README.md)



## More

If you want to learn more, please refer to the following:

- [Elastos_Development_Guide](DevDoc/References/Elastos_Development_Guide.md)
- [Elastos_64bit_Upgrade_Guide](DevDoc/References/Elastos_64bit_Upgrade_Guide.md)
- [How_to_Export_API](DevDoc/DocTools/How_to_Export_API.md)