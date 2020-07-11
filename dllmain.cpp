#include <Windows.h>
#include <PluginAPI/Plugin.h>
#include <slang.h>

#include <stdio.h>
#include <vector>
#include <utility>
#include <string>

#ifdef WIN32
# define FEXPORT __declspec(dllexport)
#else
# define FEXPORT 
#endif

namespace sl
{
	class SlangPlugin : public ed::IPlugin1
	{
	public:
		virtual bool Init(bool isWeb, int sedVersion) {
			m_buildLangDefinition();
			return true;
		}

		virtual void InitUI(void* ctx) { }
		virtual void OnEvent(void* e) { }
		virtual void Update(float delta) { }
		virtual void Destroy() {
			if (m_shaderBlob != nullptr) {
				m_shaderBlob->release();
				m_shaderBlob = nullptr;
			}
		}

		virtual bool IsRequired() { return 0; }
		virtual bool IsVersionCompatible(int version) { return 0; }

		virtual void BeginRender() { }
		virtual void EndRender() { }

		virtual void Project_BeginLoad() { }
		virtual void Project_EndLoad() { }
		virtual void Project_BeginSave() { }
		virtual void Project_EndSave() { }
		virtual bool Project_HasAdditionalData() { return 0; }
		virtual const char* Project_ExportAdditionalData() { return 0; }
		virtual void Project_ImportAdditionalData(const char* xml) { }
		virtual void Project_CopyFilesOnSave(const char* dir) { }

		/* list: file, newproject, project, createitem, window, custom */
		virtual bool HasCustomMenuItem() { return 0; }
		virtual bool HasMenuItems(const char* name) { return 0; }
		virtual void ShowMenuItems(const char* name) { }

		/* list: pipeline, shaderpass_add (owner = ShaderPass), pluginitem_add (owner = char* ItemType, extraData = PluginItemData) objects, editcode (owner = char* ItemName) */
		virtual bool HasContextItems(const char* name) { return 0; }
		virtual void ShowContextItems(const char* name, void* owner = nullptr, void* extraData = nullptr) { }

		// system variable methods
		virtual int SystemVariables_GetNameCount(ed::plugin::VariableType varType) { return 0; }
		virtual const char* SystemVariables_GetName(ed::plugin::VariableType varType, int index) { return 0; }
		virtual bool SystemVariables_HasLastFrame(char* name, ed::plugin::VariableType varType) { return 0; }
		virtual void SystemVariables_UpdateValue(char* data, char* name, ed::plugin::VariableType varType, bool isLastFrame) { }

		// function variables
		virtual int VariableFunctions_GetNameCount(ed::plugin::VariableType vtype) { return 0; }
		virtual const char* VariableFunctions_GetName(ed::plugin::VariableType varType, int index) { return 0; }
		virtual bool VariableFunctions_ShowArgumentEdit(char* fname, char* args, ed::plugin::VariableType vtype) { return 0; }
		virtual void VariableFunctions_UpdateValue(char* data, char* args, char* fname, ed::plugin::VariableType varType) { }
		virtual int VariableFunctions_GetArgsSize(char* fname, ed::plugin::VariableType varType) { return 0; }
		virtual void VariableFunctions_InitArguments(char* args, char* fname, ed::plugin::VariableType vtype) { }
		virtual const char* VariableFunctions_ExportArguments(char* fname, ed::plugin::VariableType vtype, char* args) { return 0; }
		virtual void VariableFunctions_ImportArguments(char* fname, ed::plugin::VariableType vtype, char* args, const char* argsString) { }

		// object manager stuff
		virtual bool Object_HasPreview(const char* type) { return 0; }
		virtual void Object_ShowPreview(const char* type, void* data, unsigned int id) { }
		virtual bool Object_IsBindable(const char* type) { return 0; }
		virtual bool Object_IsBindableUAV(const char* type) { return 0; }
		virtual void Object_Remove(const char* name, const char* type, void* data, unsigned int id) { }
		virtual bool Object_HasExtendedPreview(const char* type) { return 0; }
		virtual void Object_ShowExtendedPreview(const char* type, void* data, unsigned int id) { }
		virtual bool Object_HasProperties(const char* type) { return 0; }
		virtual void Object_ShowProperties(const char* type, void* data, unsigned int id) { }
		virtual void Object_Bind(const char* type, void* data, unsigned int id) { }
		virtual const char* Object_Export(char* type, void* data, unsigned int id) { return 0; }
		virtual void Object_Import(const char* name, const char* type, const char* argsString) { }
		virtual bool Object_HasContext(const char* type) { return 0; }
		virtual void Object_ShowContext(const char* type, void* data) { }

		// pipeline item stuff
		virtual bool PipelineItem_HasProperties(const char* type, void* data) { return 0; }
		virtual void PipelineItem_ShowProperties(const char* type, void* data) { }
		virtual bool PipelineItem_IsPickable(const char* type, void* data) { return 0; }
		virtual bool PipelineItem_HasShaders(const char* type, void* data) { return 0; }
		virtual void PipelineItem_OpenInEditor(const char* type, void* data) { }
		virtual bool PipelineItem_CanHaveChild(const char* type, void* data, ed::plugin::PipelineItemType itemType) { return 0; }
		virtual int PipelineItem_GetInputLayoutSize(const char* type, void* data) { return 0; }
		virtual void PipelineItem_GetInputLayoutItem(const char* type, void* data, int index, ed::plugin::InputLayoutItem& out) { }
		virtual void PipelineItem_Remove(const char* itemName, const char* type, void* data) { }
		virtual void PipelineItem_Rename(const char* oldName, const char* newName) { }
		virtual void PipelineItem_AddChild(const char* owner, const char* name, ed::plugin::PipelineItemType type, void* data) { }
		virtual bool PipelineItem_CanHaveChildren(const char* type, void* data) { return 0; }
		virtual void* PipelineItem_CopyData(const char* type, void* data) { return 0; }
		virtual void PipelineItem_Execute(void* Owner, ed::plugin::PipelineItemType OwnerType, const char* type, void* data) { }
		virtual void PipelineItem_Execute(const char* type, void* data, void* children, int count) { }
		virtual void PipelineItem_GetWorldMatrix(const char* type, void* data, float(&pMat)[16]) { }
		virtual bool PipelineItem_Intersect(const char* type, void* data, const float* rayOrigin, const float* rayDir, float& hitDist) { return 0; }
		virtual void PipelineItem_GetBoundingBox(const char* type, void* data, float(&minPos)[3], float(&maxPos)[3]) { }
		virtual bool PipelineItem_HasContext(const char* type, void* data) { return 0; }
		virtual void PipelineItem_ShowContext(const char* type, void* data) { }
		virtual const char* PipelineItem_Export(const char* type, void* data) { return 0; }
		virtual void* PipelineItem_Import(const char* ownerName, const char* name, const char* type, const char* argsString) { return 0; }
		virtual void PipelineItem_MoveDown(void* ownerData, const char* ownerType, const char* itemName) { }
		virtual void PipelineItem_MoveUp(void* ownerData, const char* ownerType, const char* itemName) { }
		virtual void PipelineItem_ApplyGizmoTransform(const char* type, void* data, float* transl, float* scale, float* rota) { }
		virtual void PipelineItem_GetTransform(const char* type, void* data, float* transl, float* scale, float* rota) { }
		virtual void PipelineItem_DebugVertexExecute(void* Owner, ed::plugin::PipelineItemType OwnerType, const char* type, void* data, unsigned int colorVarLoc) { }
		virtual int PipelineItem_DebugVertexExecute(const char* type, void* data, const char* childName, float rx, float ry, int vertexGroup) { return 0; }
		virtual void PipelineItem_DebugInstanceExecute(void* Owner, ed::plugin::PipelineItemType OwnerType, const char* type, void* data, unsigned int colorVarLoc) { }
		virtual int PipelineItem_DebugInstanceExecute(const char* type, void* data, const char* childName, float rx, float ry, int vertexGroup) { return 0; }
		virtual unsigned int PipelineItem_GetVBO(const char* type, void* data) { return 0; }
		virtual unsigned int PipelineItem_GetVBOStride(const char* type, void* data) { return 0; }
		virtual bool PipelineItem_CanChangeVariables(const char* type, void* data) { return 0; }
		virtual bool PipelineItem_IsDebuggable(const char* type, void* data) { return 0; }
		virtual bool PipelineItem_IsStageDebuggable(const char* type, void* data, ed::plugin::ShaderStage stage) { return 0; }
		virtual void PipelineItem_DebugExecute(const char* type, void* data, void* children, int count, int* debugID) { }
		virtual unsigned int PipelineItem_GetTopology(const char* type, void* data) { return 0; }
		virtual unsigned int PipelineItem_GetVariableCount(const char* type, void* data) { return 0; }
		virtual const char* PipelineItem_GetVariableName(const char* type, void* data, unsigned int variable) { return 0; }
		virtual ed::plugin::VariableType PipelineItem_GetVariableType(const char* type, void* data, unsigned int variable) { return ed::plugin::VariableType::Float1; }
		virtual float PipelineItem_GetVariableValueFloat(const char* type, void* data, unsigned int variable, int col, int row) { return 0; }
		virtual int PipelineItem_GetVariableValueInteger(const char* type, void* data, unsigned int variable, int col) { return 0; }
		virtual bool PipelineItem_GetVariableValueBoolean(const char* type, void* data, unsigned int variable, int col) { return 0; }
		virtual unsigned int PipelineItem_GetSPIRVSize(const char* type, void* data, ed::plugin::ShaderStage stage) { return 0; }
		virtual unsigned int* PipelineItem_GetSPIRV(const char* type, void* data, ed::plugin::ShaderStage stage) { return 0; }
		virtual void PipelineItem_DebugPrepareVariables(const char* type, void* data, const char* name) { }
		virtual bool PipelineItem_DebugUsesCustomTextures(const char* type, void* data) { return 0; }
		virtual unsigned int PipelineItem_DebugGetTexture(const char* type, void* data, int loc, const char* variableName) { return 0; }
		virtual void PipelineItem_DebugGetTextureSize(const char* type, void* data, int loc, const char* variableName, int& x, int& y, int& z) { }

		// options
		virtual bool Options_HasSection() { return 0; }
		virtual void Options_RenderSection() { }
		virtual void Options_Parse(const char* key, const char* val) { }
		virtual int Options_GetCount() { return 0; }
		virtual const char* Options_GetKey(int index) { return 0; }
		virtual const char* Options_GetValue(int index) { return 0; }

		// languages
		virtual int CustomLanguage_GetCount() { return 1; }
		virtual const char* CustomLanguage_GetName(int langID) { return "Slang"; }
		virtual const unsigned int* CustomLanguage_CompileToSPIRV(int langID, const char* src, size_t src_len, ed::plugin::ShaderStage stage, const char* entry, ed::plugin::ShaderMacro* macros, size_t macroCount, size_t* spv_length, bool* compiled)
		{
			SlangSession* slangSession = spCreateSession(NULL);
			SlangCompileRequest* slangRequest = spCreateCompileRequest(slangSession);

			int targetIndex = spAddCodeGenTarget(slangRequest, SLANG_SPIRV);

			spSetTargetProfile(slangRequest, targetIndex, spFindProfile(slangSession, "glsl_330"));
			spSetDebugInfoLevel(slangRequest, SLANG_DEBUG_INFO_LEVEL_STANDARD);

			int translationUnitIndex = spAddTranslationUnit(slangRequest, SLANG_SOURCE_LANGUAGE_SLANG, nullptr);

			spAddTranslationUnitSourceString(slangRequest, translationUnitIndex, "shader.slang", src);

			SlangStage slangStage = SLANG_STAGE_VERTEX;
			if (stage == ed::plugin::ShaderStage::Pixel)
				slangStage = SLANG_STAGE_PIXEL;
			else if (stage == ed::plugin::ShaderStage::Geometry)
				slangStage = SLANG_STAGE_GEOMETRY;

			int vertexIndex = spAddEntryPoint(slangRequest, translationUnitIndex, entry, slangStage);

			for (int i = 0; i < macroCount; i++)
				if (macros[i].Active)
					spAddPreprocessorDefine(slangRequest, macros[i].Name, macros[i].Value);

			int includePathCount = this->GetIncludePathCount();
			char path[512] = { 0 };
			for (int i = 0; i < includePathCount; i++) {
				path[0] = 0;
				const char* includePath = GetIncludePath(Project, i);
				GetProjectPath(Project, includePath, path);

				spAddSearchPath(slangRequest, path);
			}

			const SlangResult compileRes = spCompile(slangRequest);
			*compiled = false;

			if (auto diagnostics = spGetDiagnosticOutput(slangRequest))
			{
				std::string errMsg(diagnostics);

				if (errMsg.size() > 0) {
					int line = -1;
					std::string msg = "";

					size_t lineLocBegin = errMsg.find_first_of('(');
					size_t lineLocEnd = errMsg.find_first_of(')');
					if (lineLocBegin != std::string::npos) {
						std::string lineStr = errMsg.substr(lineLocBegin + 1, lineLocEnd - lineLocBegin - 1);
						msg = errMsg.substr(lineLocEnd + 3);

						line = std::stoi(lineStr);
					}

					if (lineLocBegin != std::string::npos)
						this->AddMessage(this->Messages, ed::plugin::MessageType::Error, GetMessagesCurrentItem(Messages), msg.c_str(), line);
				}
			}

			// If compilation failed, there is no point in continuing any further.
			if (SLANG_FAILED(compileRes))
			{
				spDestroyCompileRequest(slangRequest);
				spDestroySession(slangSession);
				return nullptr;
			}

			*compiled = true;

			if (m_shaderBlob != nullptr)
				m_shaderBlob->release();
			m_shaderBlob = nullptr;

			spGetEntryPointCodeBlob(slangRequest, vertexIndex, 0, &m_shaderBlob);

			spDestroyCompileRequest(slangRequest);
			spDestroySession(slangSession);

			*spv_length = m_shaderBlob->getBufferSize() / 4;

			const unsigned int* spvData = (unsigned int const*)m_shaderBlob->getBufferPointer();

			return spvData;
		}
		virtual const char* CustomLanguage_ProcessGeneratedGLSL(int langID, const char* srcPtr)
		{
			m_tempSrc = std::string(srcPtr);
			std::string& src = m_tempSrc;

			size_t slangParamGroupLoc = src.find("struct SLANG_ParameterGroup_");
			while (slangParamGroupLoc != std::string::npos) {
				size_t slangParamGroupOpen = src.find_first_of('{', slangParamGroupLoc);
				size_t slangParamGroupNameEnd = src.find_first_of('\n', slangParamGroupLoc);

				size_t nameBegin = slangParamGroupLoc + strlen("struct SLANG_ParameterGroup_");
				std::string structName = src.substr(nameBegin, slangParamGroupNameEnd - nameBegin);

				size_t unifIndex = slangParamGroupLoc;

				/* STRUCT DEFINITION */
				src.erase(slangParamGroupLoc, slangParamGroupOpen - slangParamGroupLoc + 2); // delete structure definition
				while (src[unifIndex] != '}') {
					src.insert(unifIndex, "uniform"); // add "uniform" before each element

					// delete "_0"
					size_t suffixEndLoc = src.find(';', unifIndex);
					src.erase(suffixEndLoc - 2, 2);

					unifIndex = src.find_first_of('\n', unifIndex + 1) + 1;
				}
				src.erase(unifIndex, 3); // delete "};"

				/* GET NAME */
				size_t defNameBegin = src.find("SLANG_ParameterGroup_" + structName);
				size_t defNameEnd = src.find(';', defNameBegin);
				defNameBegin += strlen(("SLANG_ParameterGroup_" + structName).c_str()) + 1;
				std::string defName = src.substr(defNameBegin, defNameEnd - defNameBegin);

				size_t blockNameBegin = src.find_first_of('}', defNameBegin);
				size_t blockNameEnd = src.find_first_of('\n', blockNameBegin);
				std::string blockName = src.substr(blockNameBegin + 2, blockNameEnd - blockNameBegin - 3);

				size_t blockBeginLoc = src.rfind("layout(binding = ", defNameBegin);
				src.erase(blockBeginLoc, blockNameEnd - blockBeginLoc);

				std::string name = blockName + "." + defName + ".";
				size_t nameLoc = src.find(name);
				while (nameLoc != std::string::npos) {
					src.erase(nameLoc, name.size()); // erase "cb.name."

					size_t suffixLoc = src.find('_', nameLoc);
					src.erase(suffixLoc, 2);

					nameLoc = src.find(name);
				}

				slangParamGroupLoc = src.find("struct SLANG_ParameterGroup_");
			}

			return m_tempSrc.c_str();
		}
		virtual bool CustomLanguage_SupportsAutoUniforms(int langID) { return false; }
		virtual bool CustomLanguage_IsDebuggable(int langID) { return false; }
		virtual const char* CustomLanguage_GetDefaultExtension(int langID) { return "slang"; }

		// language text editor
		virtual bool ShaderEditor_Supports(int langID) { return 0; }
		virtual void ShaderEditor_Open(int langID, int editorID, const char* data, int dataLen) { }
		virtual void ShaderEditor_Render(int langID, int editorID) { }
		virtual void ShaderEditor_Close(int langID, int editorID) { }
		virtual const char* ShaderEditor_GetContent(int langID, int editorID, size_t* dataLength) { return 0; }
		virtual bool ShaderEditor_IsChanged(int langID, int editorID) { return 0; }
		virtual void ShaderEditor_ResetChangeState(int langID, int editorID) { }
		virtual bool ShaderEditor_CanUndo(int langID, int editorID) { return 0; }
		virtual bool ShaderEditor_CanRedo(int langID, int editorID) { return 0; }
		virtual void ShaderEditor_Undo(int langID, int editorID) { }
		virtual void ShaderEditor_Redo(int langID, int editorID) { }
		virtual void ShaderEditor_Cut(int langID, int editorID) { }
		virtual void ShaderEditor_Paste(int langID, int editorID) { }
		virtual void ShaderEditor_Copy(int langID, int editorID) { }
		virtual void ShaderEditor_SelectAll(int langID, int editorID) { }
		virtual bool ShaderEditor_HasStats(int langID, int editorID) { return 0; }

		// code editor
		virtual void CodeEditor_SaveItem(const char* src, int srcLen, int id) {}
		virtual void CodeEditor_CloseItem(int id) {}
		virtual bool LanguageDefinition_Exists(int id) { return true; }
		virtual int LanguageDefinition_GetKeywordCount(int id)
		{
			return m_langDefKeywords.size();
		}
		virtual const char** LanguageDefinition_GetKeywords(int id)
		{
			return m_langDefKeywords.data();
		}
		virtual int LanguageDefinition_GetTokenRegexCount(int id)
		{
			return m_langDefRegex.size();
		}
		virtual const char* LanguageDefinition_GetTokenRegex(int index, ed::plugin::TextEditorPaletteIndex& palIndex, int id)
		{
			palIndex = m_langDefRegex[index].second;
			return m_langDefRegex[index].first;
		}
		virtual int LanguageDefinition_GetIdentifierCount(int id)
		{
			return m_langDefIdentifiers.size();
		}
		virtual const char* LanguageDefinition_GetIdentifier(int index, int id)
		{
			return m_langDefIdentifiers[index].first;
		}
		virtual const char* LanguageDefinition_GetIdentifierDesc(int index, int id)
		{
			return m_langDefIdentifiers[index].second;
		}
		virtual const char* LanguageDefinition_GetCommentStart(int id)
		{
			return "/*";
		}
		virtual const char* LanguageDefinition_GetCommentEnd(int id)
		{
			return "*/";
		}
		virtual const char* LanguageDefinition_GetLineComment(int id)
		{
			return "//";
		}
		virtual bool LanguageDefinition_IsCaseSensitive(int id) { return true; }
		virtual bool LanguageDefinition_GetAutoIndent(int id) { return true; }
		virtual const char* LanguageDefinition_GetName(int id) { return "Slang"; }
		virtual const char* LanguageDefinition_GetNameAbbreviation(int id) { return "SL"; }

		// autocomplete
		virtual int Autocomplete_GetCount(ed::plugin::ShaderStage stage) { return 0; }
		virtual const char* Autocomplete_GetDisplayString(ed::plugin::ShaderStage stage, int index) { return 0; }
		virtual const char* Autocomplete_GetSearchString(ed::plugin::ShaderStage stage, int index) { return 0; }
		virtual const char* Autocomplete_GetValue(ed::plugin::ShaderStage stage, int index) { return 0; }

		// file change checks
		virtual int ShaderFilePath_GetCount() { return 0; }
		virtual const char* ShaderFilePath_Get(int index) { return 0; }
		virtual bool ShaderFilePath_HasChanged() { return 0; }
		virtual void ShaderFilePath_Update() {}

		// misc
		virtual bool HandleDropFile(const char* filename) { return 0; }
		virtual void HandleRecompile(const char* itemName) {}
		virtual void HandleRecompileFromSource(const char* itemName, int sid, const char* shaderCode, int shaderSize) {}
		virtual void HandleShortcut(const char* name) {}
		virtual void HandlePluginMessage(const char* sender, char* msg, int msgLen) {}
		virtual void HandleApplicationEvent(ed::plugin::ApplicationEvent event, void* data1, void* data2) {}
		virtual void HandleNotification(int id) {}

	private:
		std::vector<const char*> m_langDefKeywords;
		std::vector<std::pair<const char*, ed::plugin::TextEditorPaletteIndex>> m_langDefRegex;
		std::vector<std::pair<const char*, const char*>> m_langDefIdentifiers;
		void m_buildLangDefinition()
		{
			// keywords
			m_langDefKeywords.clear();
			m_langDefKeywords = {
				"AppendStructuredBuffer", "asm", "asm_fragment", "BlendState", "bool", "break", "Buffer", "ByteAddressBuffer", "case", "cbuffer", "centroid", "class", "column_major", "compile", "compile_fragment",
				"CompileShader", "const", "continue", "ComputeShader", "ConsumeStructuredBuffer", "default", "DepthStencilState", "DepthStencilView", "discard", "do", "double", "DomainShader", "dword", "else",
				"export", "extern", "false", "float", "for", "fxgroup", "GeometryShader", "groupshared", "half", "Hullshader", "if", "in", "inline", "inout", "InputPatch", "int", "interface", "line", "lineadj",
				"linear", "LineStream", "matrix", "min16float", "min10float", "min16int", "min12int", "min16uint", "namespace", "nointerpolation", "noperspective", "NULL", "out", "OutputPatch", "packoffset",
				"pass", "pixelfragment", "PixelShader", "point", "PointStream", "precise", "RasterizerState", "RenderTargetView", "return", "register", "row_major", "RWBuffer", "RWByteAddressBuffer", "RWStructuredBuffer",
				"RWTexture1D", "RWTexture1DArray", "RWTexture2D", "RWTexture2DArray", "RWTexture3D", "sample", "sampler", "SamplerState", "SamplerComparisonState", "shared", "snorm", "stateblock", "stateblock_state",
				"static", "string", "struct", "switch", "StructuredBuffer", "tbuffer", "technique", "technique10", "technique11", "texture", "Texture1D", "Texture1DArray", "Texture2D", "Texture2DArray", "Texture2DMS",
				"Texture2DMSArray", "Texture3D", "TextureCube", "TextureCubeArray", "true", "typedef", "triangle", "triangleadj", "TriangleStream", "uint", "uniform", "unorm", "unsigned", "vector", "vertexfragment",
				"VertexShader", "void", "volatile", "while",
				"bool1","bool2","bool3","bool4","double1","double2","double3","double4", "float1", "float2", "float3", "float4", "int1", "int2", "int3", "int4", "in", "out", "inout",
				"uint1", "uint2", "uint3", "uint4", "dword1", "dword2", "dword3", "dword4", "half1", "half2", "half3", "half4",
				"float1x1","float2x1","float3x1","float4x1","float1x2","float2x2","float3x2","float4x2",
				"float1x3","float2x3","float3x3","float4x3","float1x4","float2x4","float3x4","float4x4",
				"half1x1","half2x1","half3x1","half4x1","half1x2","half2x2","half3x2","half4x2",
				"half1x3","half2x3","half3x3","half4x3","half1x4","half2x4","half3x4","half4x4",

				"interface", "import", "this"
			};

			// regex
			m_langDefRegex.clear();
			m_langDefRegex.push_back(std::make_pair("[ \\t]*#[ \\t]*[a-zA-Z_]+", ed::plugin::TextEditorPaletteIndex::Preprocessor));
			m_langDefRegex.push_back(std::make_pair("L?\\\"(\\\\.|[^\\\"])*\\\"", ed::plugin::TextEditorPaletteIndex::String));
			m_langDefRegex.push_back(std::make_pair("\\'\\\\?[^\\']\\'", ed::plugin::TextEditorPaletteIndex::CharLiteral));
			m_langDefRegex.push_back(std::make_pair("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", ed::plugin::TextEditorPaletteIndex::Number));
			m_langDefRegex.push_back(std::make_pair("[+-]?[0-9]+[Uu]?[lL]?[lL]?", ed::plugin::TextEditorPaletteIndex::Number));
			m_langDefRegex.push_back(std::make_pair("0[0-7]+[Uu]?[lL]?[lL]?", ed::plugin::TextEditorPaletteIndex::Number));
			m_langDefRegex.push_back(std::make_pair("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", ed::plugin::TextEditorPaletteIndex::Number));
			m_langDefRegex.push_back(std::make_pair("[a-zA-Z_][a-zA-Z0-9_]*", ed::plugin::TextEditorPaletteIndex::Identifier));
			m_langDefRegex.push_back(std::make_pair("[\\[\\]\\{\\}\\!\\%\\^\\&\\*\\(\\)\\-\\+\\=\\~\\|\\<\\>\\?\\/\\;\\,\\.]", ed::plugin::TextEditorPaletteIndex::Punctuation));

			// identifiers
			m_langDefIdentifiers.clear();
			m_langDefIdentifiers.push_back(std::make_pair("abort", "Terminates the current draw or dispatch call being executed."));
			m_langDefIdentifiers.push_back(std::make_pair("abs", "Absolute value (per component)."));
			m_langDefIdentifiers.push_back(std::make_pair("acos", "Returns the arccosine of each component of x."));
			m_langDefIdentifiers.push_back(std::make_pair("all", "Test if all components of x are nonzero."));
			m_langDefIdentifiers.push_back(std::make_pair("AllMemoryBarrier", "Blocks execution of all threads in a group until all memory accesses have been completed."));
			m_langDefIdentifiers.push_back(std::make_pair("AllMemoryBarrierWithGroupSync", "Blocks execution of all threads in a group until all memory accesses have been completed and all threads in the group have reached this call."));
			m_langDefIdentifiers.push_back(std::make_pair("any", "Test if any component of x is nonzero."));
			m_langDefIdentifiers.push_back(std::make_pair("asdouble", "Reinterprets a cast value into a double."));
			m_langDefIdentifiers.push_back(std::make_pair("asfloat", "Convert the input type to a float."));
			m_langDefIdentifiers.push_back(std::make_pair("asin", "Returns the arcsine of each component of x."));
			m_langDefIdentifiers.push_back(std::make_pair("asint", "Convert the input type to an integer."));
			m_langDefIdentifiers.push_back(std::make_pair("asuint", "Convert the input type to an unsigned integer."));
			m_langDefIdentifiers.push_back(std::make_pair("atan", "Returns the arctangent of x."));
			m_langDefIdentifiers.push_back(std::make_pair("atan2", "Returns the arctangent of of two values (x,y)."));
			m_langDefIdentifiers.push_back(std::make_pair("ceil", "Returns the smallest integer which is greater than or equal to x."));
			m_langDefIdentifiers.push_back(std::make_pair("CheckAccessFullyMapped", "Determines whether all values from a Sample or Load operation accessed mapped tiles in a tiled resource."));
			m_langDefIdentifiers.push_back(std::make_pair("clamp", "Clamps x to the range [min, max]."));
			m_langDefIdentifiers.push_back(std::make_pair("clip", "Discards the current pixel, if any component of x is less than zero."));
			m_langDefIdentifiers.push_back(std::make_pair("cos", "Returns the cosine of x."));
			m_langDefIdentifiers.push_back(std::make_pair("cosh", "Returns the hyperbolic cosine of x."));
			m_langDefIdentifiers.push_back(std::make_pair("countbits", "Counts the number of bits (per component) in the input integer."));
			m_langDefIdentifiers.push_back(std::make_pair("cross", "Returns the cross product of two 3D vectors."));
			m_langDefIdentifiers.push_back(std::make_pair("D3DCOLORtoUBYTE4", "Swizzles and scales components of the 4D vector x to compensate for the lack of UBYTE4 support in some hardware."));
			m_langDefIdentifiers.push_back(std::make_pair("ddx", "Returns the partial derivative of x with respect to the screen-space x-coordinate."));
			m_langDefIdentifiers.push_back(std::make_pair("ddx_coarse", "Computes a low precision partial derivative with respect to the screen-space x-coordinate."));
			m_langDefIdentifiers.push_back(std::make_pair("ddx_fine", "Computes a high precision partial derivative with respect to the screen-space x-coordinate."));
			m_langDefIdentifiers.push_back(std::make_pair("ddy", "Returns the partial derivative of x with respect to the screen-space y-coordinate."));
			m_langDefIdentifiers.push_back(std::make_pair("ddy_coarse", "Returns the partial derivative of x with respect to the screen-space y-coordinate."));
			m_langDefIdentifiers.push_back(std::make_pair("ddy_fine", "Computes a high precision partial derivative with respect to the screen-space y-coordinate."));
			m_langDefIdentifiers.push_back(std::make_pair("degrees", "Converts x from radians to degrees."));
			m_langDefIdentifiers.push_back(std::make_pair("determinant", "Returns the determinant of the square matrix m."));
			m_langDefIdentifiers.push_back(std::make_pair("DeviceMemoryBarrier", "Blocks execution of all threads in a group until all device memory accesses have been completed."));
			m_langDefIdentifiers.push_back(std::make_pair("DeviceMemoryBarrierWithGroupSync", "Blocks execution of all threads in a group until all device memory accesses have been completed and all threads in the group have reached this call."));
			m_langDefIdentifiers.push_back(std::make_pair("distance", "Returns the distance between two points."));
			m_langDefIdentifiers.push_back(std::make_pair("dot", "Returns the dot product of two vectors."));
			m_langDefIdentifiers.push_back(std::make_pair("dst", "Calculates a distance vector."));
			m_langDefIdentifiers.push_back(std::make_pair("errorf", "Submits an error message to the information queue."));
			m_langDefIdentifiers.push_back(std::make_pair("EvaluateAttributeAtCentroid", "Evaluates at the pixel centroid."));
			m_langDefIdentifiers.push_back(std::make_pair("EvaluateAttributeAtSample", "Evaluates at the indexed sample location."));
			m_langDefIdentifiers.push_back(std::make_pair("EvaluateAttributeSnapped", "Evaluates at the pixel centroid with an offset."));
			m_langDefIdentifiers.push_back(std::make_pair("exp", "Returns the base-e exponent."));
			m_langDefIdentifiers.push_back(std::make_pair("exp2", "Base 2 exponent(per component)."));
			m_langDefIdentifiers.push_back(std::make_pair("f16tof32", "Converts the float16 stored in the low-half of the uint to a float."));
			m_langDefIdentifiers.push_back(std::make_pair("f32tof16", "Converts an input into a float16 type."));
			m_langDefIdentifiers.push_back(std::make_pair("faceforward", "Returns -n * sign(dot(i, ng))."));
			m_langDefIdentifiers.push_back(std::make_pair("firstbithigh", "Gets the location of the first set bit starting from the highest order bit and working downward, per component."));
			m_langDefIdentifiers.push_back(std::make_pair("firstbitlow", "Returns the location of the first set bit starting from the lowest order bit and working upward, per component."));
			m_langDefIdentifiers.push_back(std::make_pair("floor", "Returns the greatest integer which is less than or equal to x."));
			m_langDefIdentifiers.push_back(std::make_pair("fma", "Returns the double-precision fused multiply-addition of a * b + c."));
			m_langDefIdentifiers.push_back(std::make_pair("fmod", "Returns the floating point remainder of x/y."));
			m_langDefIdentifiers.push_back(std::make_pair("frac", "Returns the fractional part of x."));
			m_langDefIdentifiers.push_back(std::make_pair("frexp", "Returns the mantissa and exponent of x."));
			m_langDefIdentifiers.push_back(std::make_pair("fwidth", "Returns abs(ddx(x)) + abs(ddy(x))"));
			m_langDefIdentifiers.push_back(std::make_pair("GetRenderTargetSampleCount", "Returns the number of render-target samples."));
			m_langDefIdentifiers.push_back(std::make_pair("GetRenderTargetSamplePosition", "Returns a sample position (x,y) for a given sample index."));
			m_langDefIdentifiers.push_back(std::make_pair("GroupMemoryBarrier", "Blocks execution of all threads in a group until all group shared accesses have been completed."));
			m_langDefIdentifiers.push_back(std::make_pair("GroupMemoryBarrierWithGroupSync", "Blocks execution of all threads in a group until all group shared accesses have been completed and all threads in the group have reached this call."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedAdd", "Performs a guaranteed atomic add of value to the dest resource variable."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedAnd", "Performs a guaranteed atomic and."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedCompareExchange", "Atomically compares the input to the comparison value and exchanges the result."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedCompareStore", "Atomically compares the input to the comparison value."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedExchange", "Assigns value to dest and returns the original value."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedMax", "Performs a guaranteed atomic max."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedMin", "Performs a guaranteed atomic min."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedOr", "Performs a guaranteed atomic or."));
			m_langDefIdentifiers.push_back(std::make_pair("InterlockedXor", "Performs a guaranteed atomic xor."));
			m_langDefIdentifiers.push_back(std::make_pair("isfinite", "Returns true if x is finite, false otherwise."));
			m_langDefIdentifiers.push_back(std::make_pair("isinf", "Returns true if x is +INF or -INF, false otherwise."));
			m_langDefIdentifiers.push_back(std::make_pair("isnan", "Returns true if x is NAN or QNAN, false otherwise."));
			m_langDefIdentifiers.push_back(std::make_pair("ldexp", "Returns x * 2exp"));
			m_langDefIdentifiers.push_back(std::make_pair("length", "Returns the length of the vector v."));
			m_langDefIdentifiers.push_back(std::make_pair("lerp", "Returns x + s(y - x)."));
			m_langDefIdentifiers.push_back(std::make_pair("lit", "Returns a lighting vector (ambient, diffuse, specular, 1)"));
			m_langDefIdentifiers.push_back(std::make_pair("log", "Returns the base-e logarithm of x."));
			m_langDefIdentifiers.push_back(std::make_pair("log10", "Returns the base-10 logarithm of x."));
			m_langDefIdentifiers.push_back(std::make_pair("log2", "Returns the base - 2 logarithm of x."));
			m_langDefIdentifiers.push_back(std::make_pair("mad", "Performs an arithmetic multiply/add operation on three values."));
			m_langDefIdentifiers.push_back(std::make_pair("max", "Selects the greater of x and y."));
			m_langDefIdentifiers.push_back(std::make_pair("min", "Selects the lesser of x and y."));
			m_langDefIdentifiers.push_back(std::make_pair("modf", "Splits the value x into fractional and integer parts."));
			m_langDefIdentifiers.push_back(std::make_pair("msad4", "Compares a 4-byte reference value and an 8-byte source value and accumulates a vector of 4 sums."));
			m_langDefIdentifiers.push_back(std::make_pair("mul", "Performs matrix multiplication using x and y."));
			m_langDefIdentifiers.push_back(std::make_pair("noise", "Generates a random value using the Perlin-noise algorithm."));
			m_langDefIdentifiers.push_back(std::make_pair("normalize", "Returns a normalized vector."));
			m_langDefIdentifiers.push_back(std::make_pair("pow", "Returns x^n."));
			m_langDefIdentifiers.push_back(std::make_pair("printf", "Submits a custom shader message to the information queue."));
			m_langDefIdentifiers.push_back(std::make_pair("Process2DQuadTessFactorsAvg", "Generates the corrected tessellation factors for a quad patch."));
			m_langDefIdentifiers.push_back(std::make_pair("Process2DQuadTessFactorsMax", "Generates the corrected tessellation factors for a quad patch."));
			m_langDefIdentifiers.push_back(std::make_pair("Process2DQuadTessFactorsMin", "Generates the corrected tessellation factors for a quad patch."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessIsolineTessFactors", "Generates the rounded tessellation factors for an isoline."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessQuadTessFactorsAvg", "Generates the corrected tessellation factors for a quad patch."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessQuadTessFactorsMax", "Generates the corrected tessellation factors for a quad patch."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessQuadTessFactorsMin", "Generates the corrected tessellation factors for a quad patch."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessTriTessFactorsAvg", "Generates the corrected tessellation factors for a tri patch."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessTriTessFactorsMax", "Generates the corrected tessellation factors for a tri patch."));
			m_langDefIdentifiers.push_back(std::make_pair("ProcessTriTessFactorsMin", "Generates the corrected tessellation factors for a tri patch."));
			m_langDefIdentifiers.push_back(std::make_pair("radians", "Converts x from degrees to radians."));
			m_langDefIdentifiers.push_back(std::make_pair("rcp", "Calculates a fast, approximate, per-component reciprocal."));
			m_langDefIdentifiers.push_back(std::make_pair("reflect", "Returns a reflection vector."));
			m_langDefIdentifiers.push_back(std::make_pair("refract", "Returns the refraction vector."));
			m_langDefIdentifiers.push_back(std::make_pair("reversebits", "Reverses the order of the bits, per component."));
			m_langDefIdentifiers.push_back(std::make_pair("round", "Rounds x to the nearest integer"));
			m_langDefIdentifiers.push_back(std::make_pair("rsqrt", "Returns 1 / sqrt(x)"));
			m_langDefIdentifiers.push_back(std::make_pair("saturate", "Clamps x to the range [0, 1]"));
			m_langDefIdentifiers.push_back(std::make_pair("sign", "Computes the sign of x."));
			m_langDefIdentifiers.push_back(std::make_pair("sin", "Returns the sine of x"));
			m_langDefIdentifiers.push_back(std::make_pair("sincos", "Returns the sineand cosine of x."));
			m_langDefIdentifiers.push_back(std::make_pair("sinh", "Returns the hyperbolic sine of x"));
			m_langDefIdentifiers.push_back(std::make_pair("smoothstep", "Returns a smooth Hermite interpolation between 0 and 1."));
			m_langDefIdentifiers.push_back(std::make_pair("sqrt", "Square root (per component)"));
			m_langDefIdentifiers.push_back(std::make_pair("step", "Returns (x >= a) ? 1 : 0"));
			m_langDefIdentifiers.push_back(std::make_pair("tan", "Returns the tangent of x"));
			m_langDefIdentifiers.push_back(std::make_pair("tanh", "Returns the hyperbolic tangent of x"));
			m_langDefIdentifiers.push_back(std::make_pair("tex1D", "1D texture lookup."));
			m_langDefIdentifiers.push_back(std::make_pair("tex1Dbias", "1D texture lookup with bias."));
			m_langDefIdentifiers.push_back(std::make_pair("tex1Dgrad", "1D texture lookup with a gradient."));
			m_langDefIdentifiers.push_back(std::make_pair("tex1Dlod", "1D texture lookup with LOD."));
			m_langDefIdentifiers.push_back(std::make_pair("tex1Dproj", "1D texture lookup with projective divide."));
			m_langDefIdentifiers.push_back(std::make_pair("tex2D", "2D texture lookup."));
			m_langDefIdentifiers.push_back(std::make_pair("tex2Dbias", "2D texture lookup with bias."));
			m_langDefIdentifiers.push_back(std::make_pair("tex2Dgrad", "2D texture lookup with a gradient."));
			m_langDefIdentifiers.push_back(std::make_pair("tex2Dlod", "2D texture lookup with LOD."));
			m_langDefIdentifiers.push_back(std::make_pair("tex2Dproj", "2D texture lookup with projective divide."));
			m_langDefIdentifiers.push_back(std::make_pair("tex3D", "3D texture lookup."));
			m_langDefIdentifiers.push_back(std::make_pair("tex3Dbias", "3D texture lookup with bias."));
			m_langDefIdentifiers.push_back(std::make_pair("tex3Dgrad", "3D texture lookup with a gradient."));
			m_langDefIdentifiers.push_back(std::make_pair("tex3Dlod", "3D texture lookup with LOD."));
			m_langDefIdentifiers.push_back(std::make_pair("tex3Dproj", "3D texture lookup with projective divide."));
			m_langDefIdentifiers.push_back(std::make_pair("texCUBE", "Cube texture lookup."));
			m_langDefIdentifiers.push_back(std::make_pair("texCUBEbias", "Cube texture lookup with bias."));
			m_langDefIdentifiers.push_back(std::make_pair("texCUBEgrad", "Cube texture lookup with a gradient."));
			m_langDefIdentifiers.push_back(std::make_pair("texCUBElod", "Cube texture lookup with LOD."));
			m_langDefIdentifiers.push_back(std::make_pair("texCUBEproj", "Cube texture lookup with projective divide."));
			m_langDefIdentifiers.push_back(std::make_pair("transpose", "Returns the transpose of the matrix m."));
			m_langDefIdentifiers.push_back(std::make_pair("trunc", "Truncates floating-point value(s) to integer value(s)"));
		}

		std::string m_tempSrc;

		ISlangBlob* m_shaderBlob;
	};
}

extern "C" {
	FEXPORT sl::SlangPlugin* CreatePlugin() {
		return new sl::SlangPlugin();
	}
	FEXPORT void DestroyPlugin(sl::SlangPlugin* ptr) {
		delete ptr;
	}
	FEXPORT int GetPluginAPIVersion() {
		return 1;
	}
	FEXPORT int GetPluginVersion() {
		return 1;
	}
	FEXPORT const char* GetPluginName() {
		return "Slang";
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
