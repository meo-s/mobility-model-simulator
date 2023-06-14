# Mobility Model Simulator  

![application demo animation](./demo.gif)  

정찰 목적의 UAV 모빌리티 모델을 평가하기 위한 시뮬레이터  

## Usage Guide  

### Core  

``` sh
$ cmake -DCMAKE_BUILD_TYPE=Release|Debug [-DBUILD_EXTERNALS=True|False] ...
```

반드시 `CMAKE_BUILD_TYPE`을 지정하여야합니다.  

### GUI  

#### 실행 환경  

> 2023.06 기준으로 버전이 명시되지 않은 라이브러리는 최신 버전을 사용해도 문제 없습니다.  

- python == 3.9.x
- matplotlib \<= 3.5.x  
- pygame \<= 2.2  
- pygame_gui  
- numpy  
- grpcio  

#### 실행 방법  

``` sh
$ python main.py CORE_SERVER_ADDRESS
```
