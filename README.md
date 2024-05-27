# Showdown
Directx11을 사용하여 개발한 FPS 게임

## 프로젝트 소개
Lighting, Instancing, Picking, Collision 등 DirectX11의 다양한 기능들을 적용해보면서 개발한 프로젝트입니다.

## 개발 기간
23. 09 ~ 23. 12.

## 인원
1인 개발

## 개발 환경
* DirectX11 (C++)

## 기능 설명

* ### Lighting 적용
<div align="center">
   <a href="https://github.com/minkimgyu/Showdown/blob/4387e812418865dec7a7c44e2306eaf8b59ca9f7/Showdown/Project1/lightshaderclass.cpp#L56">코드 보러가기</a>
</div>

</br>
<div align="center">
   <img src="https://github.com/minkimgyu/Showdown/assets/48249824/8fbb15f3-960c-4df5-98e8-78102d941dcf" width="60%" height="60%"/>
   </br>
   Ambient, Diffuse, Specular Light를 구현해보았습니다.
</div>

</br>

* ### 자주 사용되는 오브젝트에 Instancing, Billboard 기법을 적용하여 최적화 진행
<div align="center">
   <a href="https://github.com/minkimgyu/Showdown/blob/eda1e57158215d0185b256495a6598e31751977c/Showdown/Project1/instancemodelclass.cpp#L184">코드 보러가기</a>
</div>

</br>

<div align="center">
   <img src="https://github.com/minkimgyu/Showdown/assets/48249824/a3d12db0-cf0f-4d02-884b-657b0d45d062" width="60%" height="60%"/>
</div>
  
* ### Skybox 적용
<div align="center">
   <a href="https://github.com/minkimgyu/Showdown/blob/e4be90d29ee6abf4ddd15614d99da1f876909c4f/Showdown/Project1/skyboxclass.cpp#L18">코드 보러가기</a>
</div>

</br>
<div align="center">
   dds 파일을 활용하여 Skybox를 구현하였습니다.
</div>
  
* ### AABB 방식의 Collision으로 Picking을 적용하여 총기 시스템 구현
<div align="center">
   <a href="https://github.com/minkimgyu/Showdown/blob/e4be90d29ee6abf4ddd15614d99da1f876909c4f/Showdown/Project1/pickclass.cpp#L9">코드 보러가기</a>
</div>

</br>

<div align="center">
   <img src="https://github.com/minkimgyu/Showdown/assets/48249824/cf385a23-ba0d-4c16-a8c3-1adcceeada0a" width="60%" height="60%"/>
</div>
  
* ### Particle System을 적용하여 이펙트 구현
<div align="center">
   <a href="https://github.com/minkimgyu/Showdown/blob/e4be90d29ee6abf4ddd15614d99da1f876909c4f/Showdown/Project1/ParticleSystemClass.cpp#L45">코드 보러가기</a>
   </br>
   </br>
   Particle System을 적용하여 Die Effect 출력해보았습니다.
</div>

## 회고
