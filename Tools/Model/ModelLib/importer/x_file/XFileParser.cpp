#include "XFileParser.h"
#include "izToolKit.h"

#include "zlib.h"

CXFileParser::CXFileParser()
{
	m_bIsBinary = IZ_FALSE;
	m_nFloatSize = 4;
	m_nIntBufferedNum = 0;
	m_nFloatBufferedNum = 0;

	m_bHasSkin = IZ_FALSE;

	m_nAnimTicksPerSecond = 0;
	m_nMaxAnimTime = 0;

	m_pRootNode = IZ_NULL;

	m_nReadMtrlNum = 0;
}

CXFileParser::~CXFileParser()
{
	m_MemInStream.Finalize();
	Clear();
}

void CXFileParser::Clear()
{
	for (size_t i = 0; i < m_NodeList.size(); i++) {
		SXFileNode* p = m_NodeList[i];
		delete p;
	}

	for (size_t i = 0; i < m_AnmList.size(); i++) {
		SXFileAnmSet* p = m_AnmList[i];
		delete p;
	}
}

namespace {
	struct SXFileHeader {
		IZ_CHAR magic[4];
		IZ_CHAR major_version[2];
		IZ_CHAR minor_version[2];
		IZ_CHAR format[4];
		IZ_CHAR float_type[4];
	};

	void* _alloc(void* tmp, unsigned int num, unsigned int size)
	{
		return ::calloc(num, size);
	}

	void _free(void* tmp, void* ptr)
	{
		return ::free(ptr);
	}
}	// namespace

IZ_BOOL CXFileParser::IsTxtFormat(IZ_PCSTR pszFile)
{
	FILE* fp = IZ_NULL;
	fopen_s(&fp, pszFile, "rb");

	VRETURN(fp != IZ_NULL);

	SXFileHeader sHeader;

	IZ_BOOL ret = (fread(&sHeader, 1, sizeof(sHeader), fp) == sizeof(sHeader));

	if (ret) {
		ret = ((strncmp(sHeader.format, "txt ", 4) == 0)
				|| (strncmp(sHeader.format, "tzip", 4) == 0));
	}

	fclose(fp);

	return ret;
}

// Magic identifier for MSZIP compressed data
#define MSZIP_MAGIC 0x4B43
#define MSZIP_BLOCK 32786

IZ_BOOL CXFileParser::Parse(IZ_PCSTR pszFile)
{
	// Use for parsing.
	std::vector<IZ_BYTE> uncompressed;

	// Read all data.
	izanagi::CFileInputStream cTmpStream;
	VRETURN(cTmpStream.Open(pszFile));

	std::vector<IZ_BYTE> buffer(static_cast<size_t>(cTmpStream.GetSize()));
	IZ_INPUT_READ_VRETURN(&cTmpStream, &buffer[0], 0, static_cast<size_t>(cTmpStream.GetSize()));

	cTmpStream.Finalize();

	IZ_BYTE* pStart = &buffer[0];
	IZ_BYTE* pEnd = pStart + buffer.size();

	m_MemInStream.Init(pStart, buffer.size());

	// Read header.
	SXFileHeader sHeader;
	memcpy(&sHeader, pStart, sizeof(sHeader));
	pStart += sizeof(sHeader);

	// TODO
	// Check version.

	// Compute float size.
	{
		// NOTE
		// '0' = 48 = 0x30
		m_nFloatSize = (sHeader.float_type[0] - 0x30) * 1000
						+ (sHeader.float_type[1] - 0x30) * 100
						+ (sHeader.float_type[2] - 0x30) * 10
						+ (sHeader.float_type[3] - 0x30);
		m_nFloatSize >>= 3;
	}

	IZ_BOOL bIsCompressed = IZ_FALSE;

	// Check format.
	if (strncmp(sHeader.format, "txt ", 4) == 0) {
		m_bIsBinary = IZ_FALSE;
	}
	else if (strncmp(sHeader.format, "bin ", 4) == 0) {
		m_bIsBinary = IZ_TRUE;
	}
	else if (strncmp(sHeader.format, "tzip", 4) == 0) {
		m_bIsBinary = IZ_FALSE;
		bIsCompressed = IZ_TRUE;

		// TODO
		IZ_ASSERT(IZ_FALSE);
	}
	else if (strncmp(sHeader.format, "bzip", 4) == 0) {
		m_bIsBinary = IZ_TRUE;
		bIsCompressed = IZ_TRUE;
	}

	if (bIsCompressed) {
		// build a zlib stream
		z_stream stream;
		stream.opaque = IZ_NULL;
		stream.zalloc = &_alloc;
		stream.zfree  = &_free;
		stream.data_type = Z_BINARY;

		// initialize the inflation algorithm
		::inflateInit2(&stream, -MAX_WBITS);

		// skip unknown data (checksum, flags?)
		pStart += 6;

		// First find out how much storage we'll need. Count sections.
		BYTE* pTmp = pStart;
		UINT est_out = 0;
		while (pTmp < pEnd) {
			// read next offset
			USHORT ofs = *((USHORT*)pTmp);
			pTmp += 2;

			IZ_ASSERT(ofs < MSZIP_BLOCK);

			// check magic word
			USHORT magic = *((USHORT*)pTmp);
			pTmp += 2;

			IZ_ASSERT(magic == MSZIP_MAGIC);

			// and advance to the next offset
			pTmp += ofs;
			est_out += MSZIP_BLOCK; // one decompressed block is 32786 in size
		}

		uncompressed.resize(est_out);

		// Allocate storage and do the actual uncompressing
		IZ_BYTE* out = &uncompressed.front();
		while (pStart < pEnd)
		{
			IZ_USHORT ofs = *((USHORT*)pStart);
			pStart += 4;

			// push data to the stream
			stream.next_in   = (Bytef*)pStart;
			stream.avail_in  = ofs;
			stream.next_out  = (Bytef*)out;
			stream.avail_out = MSZIP_BLOCK;

			// and decompress the data ....
			int ret = ::inflate(&stream, Z_SYNC_FLUSH);
			if (ret != Z_OK && ret != Z_STREAM_END) {
				IZ_ASSERT(false);
			}

			::inflateReset(&stream);
			::inflateSetDictionary(&stream, (const Bytef*)out , MSZIP_BLOCK - stream.avail_out);

			// and advance to the next offset
			out +=  MSZIP_BLOCK - stream.avail_out;
			pStart += ofs;
		}

		m_MemInStream.Init(&uncompressed[0], uncompressed.size());

		// terminate zlib
		::inflateEnd(&stream);
	}
	else {
		// TODO
		//_ReadEndOfLine(m_MemInStream);

		VRETURN(
			m_MemInStream.Seek(sizeof(sHeader), izanagi::E_IO_STREAM_SEEK_POS_START));
	}

#if 0
	// For Debug...
	{
		FILE* fp = fopen("tmp.bin", "wb");
		fwrite(m_MemInStream.GetBuffer(), sizeof(IZ_BYTE), m_MemInStream.GetSize(), fp);
		fclose(fp);
	}
#endif

	m_nReadMtrlNum = 0;

	ParseFile();

	return IZ_TRUE;
}


IZ_BOOL CXFileParser::ParseFile()
{
	for (;;) {
		if (m_MemInStream.GetSize() == m_MemInStream.GetCurPos()) {
			// Parse all data.
			break;
		}

		std::string token = GetToken();

		if (token.length() == 0) {
			break;
		}
		
		if (token == "template") {
			VRETURN(ParseTemplate());
		}
		else if (token == "Frame") {
			VRETURN(ParseFrame(IZ_NULL));
		}
		else if (token == "AnimTicksPerSecond") {
			VRETURN(ParseAnimTicksPerSecond());
		}
		else if (token == "AnimationSet") {
			VRETURN(ParseAnimationSet());
		}
		else if (token == "}") {
			break;
		}
		else {
			IZ_ASSERT(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseTemplate()
{
	std::string strHeader = GetHeader();

	// read GUID
	std::string guid = GetToken();

	// Skip
	for (;;) {
		std::string str = GetToken();

		if (str == "}") {
			break;
		}

		VRETURN(str.length() > 0);
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseFrame(SXFileNode* pParent)
{
	std::string name = GetHeader();

	// Create a new node.
	SXFileNode* pNode = new SXFileNode(pParent);
	if (name != "{") {
		pNode->name = name;
	}

	m_NodeList.push_back(pNode);

	if (pParent != IZ_NULL) {
		pParent->children.push_back(pNode);
	}
	else {
		if (m_pRootNode != IZ_NULL) {
			m_pRootNode->children.push_back(pNode);
			pNode->parent = m_pRootNode;
		}
		else {
			m_pRootNode = pNode;
		}
	}

	// Read tokens.
	for (;;) {
		std::string token = GetToken();

		if (token == "}") {
			break;
		}
		else if (token == "Frame") {
			// Read child.
			VRETURN(ParseFrame(pNode));
		}
		else if (token == "FrameTransformMatrix") {
			VRETURN(ParseFrameTransformMatrix(pNode->mtxTransform));
		}
		else if (token == "Mesh") {
			SXFileMesh* pMesh = new SXFileMesh;
			VRETURN(ParseMesh(pMesh));
			pMesh->node = pNode;
			pNode->meshes.push_back(pMesh);
		}
		else if (token == "VertexDuplicationIndices") {
			// skip...
			VRETURN(SkipObject());
		}
		else if (token == "XSkinMeshHeader") {
			IZ_ASSERT(IZ_FALSE);
		}
		else if (token == "SkinWeights") {
			IZ_ASSERT(IZ_FALSE);
		}
		else {
			VRETURN(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseFrameTransformMatrix(izanagi::SMatrix& mtx)
{
	std::string name = GetHeader();
	IZ_ASSERT(name == "{");

	for (IZ_UINT i = 0; i < 16; i++) {
		mtx.a[i] = ReadFloat();
	}

	VRETURN(IsSemiColon());
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseMesh(SXFileMesh* pMesh)
{
	IZ_ASSERT(pMesh != IZ_NULL);

	pMesh->name = GetHeader();

	IZ_UINT nPosNum = ReadInt();
	pMesh->positions.resize(nPosNum);

	// Read positions.
	for (IZ_UINT i = 0; i < nPosNum; i++) {
		izanagi::SVector& pos = pMesh->positions[i];
		ReadVectorXYZ(pos);
		pos.w = 1.0f;
	}

	IZ_UINT nFaceNum = ReadInt();
	pMesh->pos_faces.resize(nFaceNum);

	//VRETURN(nFaceNum * 3 == nPosNum);
	
	// Read faces.
	for (IZ_UINT i = 0; i < nFaceNum; i++) {
		SXFileFace& face = pMesh->pos_faces[i];

		IZ_UINT nIdxNum = ReadInt();
		VRETURN(nIdxNum >= 3);

		// Read indices.
		for (IZ_UINT n = 0; n < nIdxNum; n++) {
			IZ_UINT idx = ReadInt();
			VRETURN(idx < nPosNum);
			face.indices.push_back(idx);
		}

		// Check separator
		VRETURN(IsSeparator());
	}

	for (;;) {
		std::string token = GetToken();

		if (token == "}") {
			break;
		}
		else if (token == "MeshNormals") {
			VRETURN(ParseMeshNormal(pMesh));
		}
		else if (token == "MeshVertexColors") {
			VRETURN(ParseMeshColor(pMesh));
		}
		else if (token == "MeshTextureCoords") {
			VRETURN(ParseMeshTexCoord(pMesh));
		}
		else if (token == "MeshMaterialList") {
			VRETURN(ParseMeshMaterialList(pMesh));
		}
		else if (token == "XSkinMeshHeader") {
			VRETURN(ParseSkinMeshHeader(pMesh));
		}
		else if (token == "SkinWeights") {
			VRETURN(ParseSkinWeights(pMesh));
		}
		else if (token == "VertexDuplicationIndices") {
			// skip...
			VRETURN(SkipObject());
		}
		else {
			VRETURN(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseMeshNormal(SXFileMesh* pMesh)
{
	IZ_ASSERT(pMesh != IZ_NULL);

	// Skip "{"
	GetHeader();

	IZ_UINT nNmlNum = ReadInt();
	pMesh->normals.resize(nNmlNum);

	// Read normals.
	for (IZ_UINT i = 0; i < nNmlNum; i++) {
		izanagi::SVector& nml = pMesh->normals[i];
		ReadVectorXYZ(nml);
	}

	IZ_UINT nFaceNum = ReadInt();
	pMesh->nml_faces.resize(nFaceNum);

	VRETURN(pMesh->nml_faces.size() == pMesh->pos_faces.size());
	
	// Read faces.
	for (IZ_UINT i = 0; i < nFaceNum; i++) {
		SXFileFace& face = pMesh->nml_faces[i];

		IZ_UINT nIdxNum = ReadInt();
		VRETURN(nIdxNum >= 3);

		// Read indices.
		for (IZ_UINT n = 0; n < nIdxNum; n++) {
			IZ_UINT idx = ReadInt();
			face.indices.push_back(idx);
		}

		// Check separator
		VRETURN(IsSeparator());
	}

	// Check terminator.
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseMeshColor(SXFileMesh* pMesh)
{
	IZ_ASSERT(pMesh != IZ_NULL);

	// Skip "{"
	GetHeader();

	IZ_UINT nClrNum = ReadInt();
	VRETURN(pMesh->positions.size() == nClrNum);

	pMesh->colors.resize(nClrNum);

	// Read colors.
	for (IZ_UINT i = 0; i < nClrNum; i++) {
		IZ_UINT idx = ReadInt();
		VRETURN(idx < nClrNum);

		pMesh->colors[idx] = ReadInt();

		// Check separator
		VRETURN(IsSeparator());
	}

	// Check terminator.
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseMeshTexCoord(SXFileMesh* pMesh)
{
	IZ_ASSERT(pMesh != IZ_NULL);

	// Skip "{"
	GetHeader();

	IZ_UINT nTexCoordNum = ReadInt();
	VRETURN(pMesh->positions.size() == nTexCoordNum);

	pMesh->texcoords.resize(nTexCoordNum);

	// Read texture coords.
	for (IZ_UINT i = 0; i < nTexCoordNum; i++) {
		ReadFloat2(
			pMesh->texcoords[i].u,
			pMesh->texcoords[i].v);
	}

	// Check terminator.
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseSkinMeshHeader(SXFileMesh* pMesh)
{
	// Skip datas...

	// Skip "{"
	GetHeader();

	IZ_INT nMaxSkinWeightsPerVertex = ReadInt();
	IZ_INT nMaxSkinWeightsPerFace = ReadInt();
	IZ_INT nNumBonesInMesh = ReadInt();

	// Check count of weights influences vertex.
	VRETURN(nMaxSkinWeightsPerVertex <= 4);

	// Check terminator.
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseSkinWeights(SXFileMesh* pMesh)
{
	m_bHasSkin = IZ_TRUE;

	// Skip "{"
	GetHeader();

	pMesh->skins.push_back(SXFileSkin());

	SXFileSkin& skin = pMesh->skins.back();

	skin.bone.name = GetTokenEx();

	// Find joint.

	IZ_INT nWeightNum = ReadInt();
	skin.vertex.resize(nWeightNum);
	skin.weight.resize(nWeightNum);

	for (IZ_INT i = 0; i < nWeightNum; i++) {
		skin.vertex[i] = ReadInt();
	}

	for (IZ_INT i = 0; i < nWeightNum; i++) {
		skin.weight[i] = ReadFloat();
	}

	for (IZ_UINT i = 0; i < 16; i++) {
		skin.bone.mtx.a[i] = ReadFloat();
	}

	m_BoneList.push_back(skin.bone);

	VRETURN(IsSemiColon());
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

namespace {
	inline IZ_PCSTR _GetTmpMtrlName(IZ_UINT idx)
	{
		static izChar buf[256];
		sprintf_s(buf, sizeof(buf), "Material_%d\0", idx);
		return buf;
	}
}	// namespace

IZ_BOOL CXFileParser::ParseMeshMaterialList(SXFileMesh* pMesh)
{
	IZ_ASSERT(pMesh != IZ_NULL);

	// Skip "{"
	GetHeader();

	// Read material count.
	IZ_UINT nMtrlNum = ReadInt();

	IZ_UINT nMtrlIdxNum = ReadInt();
	VRETURN(nMtrlIdxNum == pMesh->pos_faces.size());

	for (IZ_UINT i = 0; i < nMtrlIdxNum; i++) {
		pMesh->mtrl_idx.push_back(ReadInt());
	}

	// Read materials.
	for (;;) {
		std::string token = GetToken();

		if (token == "}") {
			break;
		}
		else if (token == "Material") {
			pMesh->mtrls.push_back(SXFileMaterial());
			SXFileMaterial& mtrl = pMesh->mtrls.back();
			VRETURN(ParseMaterial(&mtrl));

			// TODO
			// マテリアル名が"{"だったときは
			// あまりにも何なので、それっぽい名前を付ける
			if (mtrl.name.compare("{") == 0) {
				mtrl.name = _GetTmpMtrlName(m_nReadMtrlNum);
			}

			m_nReadMtrlNum++;
		}
		else {
			IZ_ASSERT(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseMaterial(SXFileMaterial* pMtrl)
{
	IZ_ASSERT(pMtrl != IZ_NULL);

	pMtrl->name = GetHeader();
	if (pMtrl->name.compare("}") == 0) {
		// This name is invalid...
		pMtrl->name.clear();
	}

	ReadRGBA(pMtrl->diffuse);
	pMtrl->specularExp = ReadFloat();
	ReadRGB(pMtrl->specular);
	ReadRGB(pMtrl->emisssion);

	for (;;) {
		std::string token = GetToken();

		if (token == "}") {
			break;
		}
		else if(token == "TextureFilename" || token == "TextureFileName") {
			pMtrl->textures.push_back(SXFileTexture());
			SXFileTexture& tex = pMtrl->textures.back();
			VRETURN(ParseTexture(&tex));
		}
		else if(token == "NormalmapFilename" || token == "NormalmapFileName") {
			pMtrl->textures.push_back(SXFileTexture());
			SXFileTexture& tex = pMtrl->textures.back();
			tex.isNormalMap = IZ_TRUE;
			VRETURN(ParseTexture(&tex));
		}
		else {
			IZ_ASSERT(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseTexture(SXFileTexture* pTex)
{
	IZ_ASSERT(pTex != IZ_NULL);

	// Skip "{"
	GetHeader();

	pTex->name = GetTokenEx();

	VRETURN(!pTex->name.empty());

	// Check terminator.
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseAnimTicksPerSecond()
{
	// Skip "{"
	GetHeader();

	m_nAnimTicksPerSecond = ReadInt();

	// Check terminator.
	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseAnimationSet()
{
	SXFileAnmSet* pAnmSet = new SXFileAnmSet;
	m_AnmList.push_back(pAnmSet);

	pAnmSet->name = GetHeader();

	for (;;) {
		std::string token = GetToken();

		if (token == "}") {
			break;
		}
		else if (token == "Animation") {
			VRETURN(ParseAnimation(pAnmSet));
		}
		else {
			VRETURN(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseAnimation(SXFileAnmSet* pAnmSet)
{
	// Skip "{"
	GetHeader();

	SXFileAnm* pAnm = new SXFileAnm;
	pAnmSet->anm.push_back(pAnm);

	for (;;) {
		std::string token = GetToken();

		if (token == "}") {
			break;
		}
		else if (token == "{") {
			// Read frame name.
			pAnm->name = GetToken();
			VRETURN(IsCloseBrace());
		}
		else if (token == "AnimationKey") {
			VRETURN(ParseAnimationKey(pAnm));
		}
		else {
			VRETURN(IZ_FALSE);
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::ParseAnimationKey(SXFileAnm* pAnm)
{
	// Skip "{"
	GetHeader();

	// Read type.
	IZ_INT nType = ReadInt();

	// Read number of keys.
	IZ_INT nNumKeys = ReadInt();

	if (pAnm->keys.empty()) {
		pAnm->keys.resize(nNumKeys);
	}
	
	VRETURN(pAnm->keys.size() == nNumKeys);

	for (IZ_INT i = 0; i < nNumKeys; i++) {
		SXFileAnmKey& sKey = pAnm->keys[i];

		IZ_INT time = ReadInt();
		if (sKey.time == -1) {
			sKey.time = time;
		}
		else {
			VRETURN(sKey.time == time);
		}

		m_nMaxAnimTime = IZ_MAX(m_nMaxAnimTime, sKey.time);

		switch (nType) {
		case 0:	// quaternion
		{
			// Read count.
			VRETURN(ReadInt() == 4);
			VRETURN(!sKey.type.mtx);

			sKey.type.quat = IZ_TRUE;

			sKey.data.pose.quat.w = ReadFloat();
			sKey.data.pose.quat.x = ReadFloat();
			sKey.data.pose.quat.y = ReadFloat();
			sKey.data.pose.quat.z = ReadFloat();

			VRETURN(IsSemiColon());
			break;
		}
		case 1:	// scale
		{
			VRETURN(ReadInt() == 3);
			VRETURN(!sKey.type.mtx);

			sKey.type.scale = IZ_TRUE;

			sKey.data.pose.scale[0] = ReadFloat();
			sKey.data.pose.scale[1] = ReadFloat();
			sKey.data.pose.scale[2] = ReadFloat();

			VRETURN(IsSemiColon());
			break;
		}
		case 2:	// position
		{
			VRETURN(ReadInt() == 3);
			VRETURN(!sKey.type.mtx);

			sKey.type.trans = IZ_TRUE;

			sKey.data.pose.trans[0] = ReadFloat();
			sKey.data.pose.trans[1] = ReadFloat();
			sKey.data.pose.trans[2] = ReadFloat();

			VRETURN(IsSemiColon());
			break;
		}
		case 3:	// matrix
		case 4:	// matrix
		{
			VRETURN(ReadInt() == 16);
			VRETURN(sKey.type.val == 0);

			sKey.type.mtx = IZ_TRUE;

			for (IZ_UINT i = 0; i < 16; i++) {
				sKey.data.mtx.a[i] = ReadFloat();
			}

			VRETURN(IsSemiColon());
			break;
		}
		default:
			VRETURN(IZ_FALSE);
		}

		VRETURN(IsSeparator());
	}

	VRETURN(IsCloseBrace());

	return IZ_TRUE;
}

IZ_BOOL CXFileParser::SkipObject()
{
	// Find opening brace.
	for (;;) {
		std::string token = GetToken();
		VRETURN(!token.empty());

		if (token == "{") {
			break;
		}
	}

	IZ_INT nBraceCounter = 1;

	// Find closing brace.
	while (nBraceCounter > 0) {
		std::string token = GetToken();
		VRETURN(!token.empty());

		if (token == "{") {
			nBraceCounter++;
		}
		else if (token == "}") {
			nBraceCounter--;
		}
	}

	return IZ_TRUE;
}

//////////////////////////////////////////////////////

namespace {
	inline IZ_BOOL _Skip(
		izanagi::CMemoryInputStream& cIn,
		IZ_UINT nOffset)
	{
		IZ_BOOL ret = cIn.Seek(nOffset, izanagi::E_IO_STREAM_SEEK_POS_CUR);
		IZ_ASSERT(ret);
		return ret;
	}

	inline std::string _ReadStringFromStream(
		izanagi::CMemoryInputStream& cIn,
		IZ_UINT nLen)
	{
		IZ_PCSTR str = reinterpret_cast<IZ_PCSTR>(cIn.GetBufferCurPos());
		_Skip(cIn, nLen);
		std::string ret(str, nLen);
		return ret;
	}
}	// namespace

namespace {
	inline void _NextOneChar(izanagi::CMemoryInputStream& cIn)
	{
		IZ_ASSERT(cIn.GetCurPos() < cIn.GetSize());
		cIn.Seek(sizeof(izChar), izanagi::E_IO_STREAM_SEEK_POS_CUR);
	}

	inline const izChar* _GetChar(izanagi::CMemoryInputStream& cIn)
	{
		VRETURN_NULL(cIn.GetCurPos() < cIn.GetSize());

		const void* p = cIn.GetBufferCurPos();
		return reinterpret_cast<const izChar*>(p);
	}

	inline izChar _GetOneChar(izanagi::CMemoryInputStream& cIn)
	{
		const izChar* ret = _GetChar(cIn);
		_NextOneChar(cIn);
		return *ret;
	}

	inline void _MoveToEndOfLine(izanagi::CMemoryInputStream& cIn)
	{
		while (cIn.GetCurPos() < cIn.GetSize()) {
			const izChar* ch = _GetChar(cIn);
			if ((ch[0] == '\n') || (ch[0] == '\r')) {
				_NextOneChar(cIn);
				return;
			}

			_NextOneChar(cIn);
		}
	}

	inline void _MoveToNextChar(izanagi::CMemoryInputStream& cIn)
	{
		for (;;) {
			izChar ch = *_GetChar(cIn);
			while ((cIn.GetCurPos() < cIn.GetSize()) && ::isspace(ch)) {
				_NextOneChar(cIn);
				if (cIn.GetCurPos() < cIn.GetSize()) {
					ch = *_GetChar(cIn);
				}
			}

			if (cIn.GetCurPos() >= cIn.GetSize()) {
				return;
			}

			// Check comment.
			const izChar* p = _GetChar(cIn);
			if (p[0] == '/') {
				if (p[1] == '/') {
					_MoveToEndOfLine(cIn);
				}
			}
			else if (p[0] == '#') {
				_MoveToEndOfLine(cIn);
			}
			else {
				break;
			}
		}
	}
}	// namespace

std::string CXFileParser::GetToken(IZ_BOOL bSkipData/*= IZ_TRUE*/)
{
	if (m_bIsBinary) {
		IZ_UINT16 nToken = Read2Byte();

		switch (nToken) {
		case 0x01:
			{
				// name.
				IZ_UINT length = Read4Byte();
				std::string ret = _ReadStringFromStream(m_MemInStream, length);
				return ret;
			}
		case 0x02:
			{
				// string
				IZ_UINT length = Read4Byte();
				std::string ret = _ReadStringFromStream(m_MemInStream, length);
				_Skip(m_MemInStream, 2);
				return ret;
			}
		case 0x03:
			// integer
			{
				if (bSkipData) {
					_Skip(m_MemInStream, 4);
				}
				return "<integer>";
			}
		case 0x05:
			// GUID
			{
				if (bSkipData) {
					_Skip(m_MemInStream, 16);
				}
				return "<guid>";
			}
		case 0x06:
			// Integer array.
			{
				if (bSkipData) {
					IZ_UINT length = Read4Byte();
					_Skip(m_MemInStream, length * sizeof(IZ_UINT32));
				}
				return "<int_list>";
			}
		case 0x07:
			// Float array.
			{
				if (bSkipData) {
					IZ_UINT length = Read4Byte();
					_Skip(m_MemInStream, length * m_nFloatSize);
				}
				return "<flt_list>";
			}
		case 0x0a:
			return "{";
		case 0x0b:
			return "}";
		case 0x0c:
			return "(";
		case 0x0d:
			return ")";
		case 0x0e:
			return "[";
		case 0x0f:
			return "]";
		case 0x10:
			return "<";
		case 0x11:
			return ">";
		case 0x12:
			return ".";
		case 0x13:
			return ",";
		case 0x14:
			return ";";
		case 0x1f:
			return "template";
		case 0x28:
			return "WORD";
		case 0x29:
			return "DWORD";
		case 0x2a:
			return "FLOAT";
		case 0x2b:
			return "DOUBLE";
		case 0x2c:
			return "CHAR";
		case 0x2d:
			return "UCHAR";
		case 0x2e:
			return "SWORD";
		case 0x2f:
			return "SDWORD";
		case 0x30:
			return "void";
		case 0x31:
			return "string";
		case 0x32:
			return "unicode";
		case 0x33:
			return "cstring";
		case 0x34:
			return "array";
		}
	}
	else {
		_MoveToNextChar(m_MemInStream);
		if (m_MemInStream.GetCurPos() >= m_MemInStream.GetSize()) {
			return "";
		}

		std::string ret;

		izChar ch = _GetOneChar(m_MemInStream);
		while ((m_MemInStream.GetCurPos() < m_MemInStream.GetSize()) && !::isspace(ch)) {
			if (ch == ';' || ch == ',' || ch == '{' || ch == '}') {
				ret += ch;
				break;
			}

			ret += ch;
			ch = _GetOneChar(m_MemInStream);
		}

		if ((m_MemInStream.GetCurPos() == m_MemInStream.GetSize())
			&& !::isspace(ch))
		{
			ret += ch;
		}

		return ret;
	}

	return "";
}

std::string CXFileParser::GetTokenEx()
{
	if (m_bIsBinary) {
		return GetToken();
	}

	_MoveToNextChar(m_MemInStream);
	IZ_ASSERT(m_MemInStream.GetCurPos() < m_MemInStream.GetSize());

	izChar ch = _GetOneChar(m_MemInStream);
	IZ_ASSERT(ch == '"');

	std::string ret;

	ch = *_GetChar(m_MemInStream);
	while ((m_MemInStream.GetCurPos() < m_MemInStream.GetSize())
			&& (ch != '"'))
	{
		ret += ch;

		_NextOneChar(m_MemInStream);
		ch = *_GetChar(m_MemInStream);
	}

	IZ_ASSERT(m_MemInStream.GetCurPos() < m_MemInStream.GetSize());

	if (ch == '"') {
		_NextOneChar(m_MemInStream);
	}

	IZ_ASSERT(IsSemiColon());

	return ret;
}

std::string CXFileParser::GetHeader()
{
	std::string strHeader = GetToken();

	if (strHeader != "{") {
		std::string tmp = GetToken();
		IZ_ASSERT(tmp == "{");
	}

	return strHeader;
}

namespace {
	inline std::string _ReadValue(izanagi::CMemoryInputStream& cIn)
	{
		std::string ret;

		IZ_BOOL bIsMinus = IZ_FALSE;

		IZ_CHAR ch = *_GetChar(cIn);
		while (ch != 0) {
			if (ch == ',' || ch == ';') {
				break;
			}

			_NextOneChar(cIn);

			// TODO
			if ((ch == 'e') || (ch == 'E')) {
				IZ_ASSERT(IZ_FALSE);
			}

			ret += ch;
			ch = *_GetChar(cIn);
		}

		return ret;
	}
}	// namespace

IZ_FLOAT CXFileParser::ReadFloat()
{
	IZ_FLOAT ret = 0.0f;

	if (m_bIsBinary) {
		if (m_nFloatBufferedNum == 0) {
			IZ_UINT nToken = Read2Byte();

			if (nToken == 0x07) {
				// integer array.
				m_nFloatBufferedNum = Read4Byte();
			}
			else {
				m_nFloatBufferedNum = 1;
			}
		}

		m_nFloatBufferedNum--;

		if (m_nFloatSize == 4) {
			IZ_INPUT_READ_ASSERT(&m_MemInStream, &ret, 0, 4);
		}
		else if (m_nFloatSize == 8) {
			IZ_DOUBLE tmp = 0.0;
			IZ_INPUT_READ_ASSERT(&m_MemInStream, &tmp, 0, 8);

			ret = static_cast<IZ_FLOAT>(tmp);
		}
	}
	else {
		// Skip space.
		_MoveToNextChar(m_MemInStream);

		std::string str = _ReadValue(m_MemInStream);

		ret = static_cast<IZ_FLOAT>(::atof(str.c_str()));

		IZ_ASSERT(IsSeparator());
	}

	return ret;
}

IZ_INT CXFileParser::ReadInt()
{
	IZ_INT ret = 0;
	
	if (m_bIsBinary) {
		if (m_nIntBufferedNum == 0) {
			IZ_UINT nToken = Read2Byte();

			if (nToken == 0x06) {
				// integer array.
				m_nIntBufferedNum = Read4Byte();
			}
			else {
				m_nIntBufferedNum = 1;
			}
		}

		m_nIntBufferedNum--;
		ret = Read4Byte();
	}
	else {
		// Skip space.
		_MoveToNextChar(m_MemInStream);

		std::string str = _ReadValue(m_MemInStream);

		ret =::atoi(str.c_str());

		IZ_ASSERT(IsSeparator());
	}

	return ret;
}

void CXFileParser::ReadVector(izanagi::SVector& vec)
{
	vec.x = ReadFloat();
	vec.y = ReadFloat();
	vec.z = ReadFloat();
	vec.w = ReadFloat();
	IZ_ASSERT(IsSeparator());
}

void CXFileParser::ReadVectorXYZ(izanagi::SVector& vec)
{
	vec.x = ReadFloat();
	vec.y = ReadFloat();
	vec.z = ReadFloat();
	IZ_ASSERT(IsSeparator());
}

void CXFileParser::ReadVectorXY(izanagi::SVector& vec)
{
	ReadFloat2(vec.x, vec.y);
}

void CXFileParser::ReadFloat2(IZ_FLOAT& f0, IZ_FLOAT& f1)
{
	f0 = ReadFloat();
	f1 = ReadFloat();
	IZ_ASSERT(IsSeparator());
}

void CXFileParser::ReadRGBA(izanagi::SVector& vec)
{
	ReadVector(vec);
}

void CXFileParser::ReadRGB(izanagi::SVector& vec)
{
	ReadVectorXYZ(vec);
}

namespace {
	template <typename _T>
	inline _T _ReadNByte(izanagi::CMemoryInputStream& cIn)
	{
		_T ret = 0;
		IZ_INPUT_READ_ASSERT(&cIn, &ret, 0, sizeof(_T));
		return ret;
	}
}	// namespace

IZ_UINT32 CXFileParser::Read4Byte()
{
	IZ_ASSERT(m_bIsBinary);

	IZ_UINT32 ret = _ReadNByte<IZ_UINT32>(m_MemInStream);
	return ret;
}


IZ_UINT16 CXFileParser::Read2Byte()
{
	IZ_ASSERT(m_bIsBinary);

	IZ_UINT16 ret = _ReadNByte<IZ_UINT16>(m_MemInStream);
	return ret;
}

IZ_BOOL CXFileParser::IsSemiColon()
{
	if (m_bIsBinary) {
		return IZ_TRUE;
	}

	std::string str = GetToken();
	return (str == ";");
}

IZ_BOOL CXFileParser::IsCloseBrace()
{
	std::string str = GetToken();
	return (str == "}");
}

IZ_BOOL CXFileParser::IsSeparator()
{
	if (m_bIsBinary) {
		return IZ_TRUE;
	}

	std::string str = GetToken();
	return (str == "," || str == ";");
}
