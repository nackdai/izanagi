* ArchiveConverter
アセットデータをパッキングした独自のデータに変換するためのツール

* ShaderCompiler
HLSLシェーダコードをGLSLシェーダコードに変換するためのツール
バックエンドとしてmojoshaderを利用している

* FontConverter
指定文字のフォントビットマップをFreeTypeでサポートしているTrueType、Type1フォント、OpenTypeなどのフォントデータから取得して
パッキングした独自のフォントデータに変換するためのツール

* EnvMapConverter
環境マップを別種類の環境マップに変換するためのツール
サポートしている環境マップの種類は以下
  - Angular
  - Mirror
  - Latitude-Longitude
  - Cross
  - Cube

* ImageBuilder
画像データを独自の画像データに変換するためのツール
特徴は以下
　- 複数の画像データを一つにパッキング
　- 変換時に指定したピクセルフォーマットに変換
　- キューブマップを指定可能
　- 変換時にミップマップを生成

* NormalMapMaker
高さマップから法線マップに変換するためのツール

* AnimationConverer
3Dモデルアニメーションデータを独自のアニメーションデータに変換するためのツール
サポートしているフォーマットは以下
　- Collada
　- X File
　- VMD（MikuMikuDance）

* MaterialConverter
3Dモデルデータに埋め込まれているマテリアルデータを独自のマテリアルデータに変換するためのツール
サポートしているフォーマットは以下
　- Collada
　- X File
　- MMD（MikuMikuDance）

* ModelConverter
3Dモデルデータから独自のメッシュデータ、ボーンデータに変換するためのツール
サポートしているフォーマットは以下
　- Collada
　- X File
　- MMD（MikuMikuDance）

* PostEffectConverter
ポストエフェクトシェーダについて記述したFXファイルから独自のポストエフェクトシェーダデータに変換するためのツール

* ShaderConverter
シェーダについて記述したFXファイルから独自のシェーダデータに変換するためのツール
