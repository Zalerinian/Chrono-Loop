#include "FBXLoader.h"
#include <fbxsdk.h>
#include <vector>

std::vector<Vertex>* verts;
std::vector<Bone>* bones;
Animation* animation;

void InitFBXLoader()
{
	verts = new std::vector<Vertex>();
	bones = new std::vector<Bone>();
	animation = new Animation();
}

void BuildMatrix(DirectX::XMFLOAT4X4* out, FbxMatrix in)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			out->m[i][j] = 0;
		}
	}

	out->m[0][0] = 1;
	out->m[1][1] = 1;
	out->m[2][2] = 1;
	out->m[3][3] = 1;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			out->m[i][j] = (float)in.mData[i][j];
		}
	}
}

void BuildHierarchy(FbxNode* inParent, int Index, int pIndex)
{
	if (inParent->GetNodeAttribute() &&
		inParent->GetNodeAttribute()->GetAttributeType() &&
		inParent->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Bone bone;
		bone.myIndex = Index;
		bone.parentIndex = pIndex;
		bones->push_back(bone);

		int cc = inParent->GetChildCount();
		for (int j = 0; j < cc; ++j)
			BuildHierarchy(inParent->GetChild(j), (int)bones->size(), Index);
	}
}

void LoadFBX(char* path, bool Reopen = true)
{
	std::ifstream isItMade;
	std::string checkPath = path;
	checkPath.replace(checkPath.find_last_of("."), 5, ".bin");
	/*checkPath + std::string(".bin");*/

	isItMade.open(checkPath.c_str() , std::ios_base::binary);
	if (!Reopen && isItMade.is_open())
		return;

	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* io = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(io);

	FbxImporter* importer;
	importer = FbxImporter::Create(manager, "");
	FbxScene* scene;
	scene = FbxScene::Create(manager, "");

	bool result = importer->Initialize(path, -1, manager->GetIOSettings());
	result = importer->Import(scene);
	importer->Destroy();

	FbxNode* root = scene->GetRootNode();
	if (root)
	{
		int children = root->GetChildCount();
		for (int i = 0; i < children; ++i)
		{
			FbxNode* child = root->GetChild(i);
			if (!child || !child->GetNodeAttribute())
				continue;

			if (child->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* childMesh = (FbxMesh*)child->GetNodeAttribute();
			FbxVector4* controlPoints = childMesh->GetControlPoints();
			FbxGeometryElementNormal* norm = childMesh->GetElementNormal(0);

			FbxVector4 translate = child->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 rotate = child->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 scale = child->GetGeometricScaling(FbxNode::eSourcePivot);
			FbxMatrix startTransform(translate, rotate, scale);

			//UV Data
			FbxStringList name;
			childMesh->GetUVSetNames(name);
			char* textureFilePath = name.GetStringAt(0);
			FbxGeometryElementUV* texture = childMesh->GetElementUV(textureFilePath);
			FbxLayerElementArrayTemplate<FbxVector2>* pUVs = 0;
			FbxArray<FbxVector4> normals;
			childMesh->GetPolygonVertexNormals(normals);
			childMesh->GetTextureUV(&pUVs, FbxLayerElement::eTextureDiffuse);
			if (!texture)
				continue;

			int points = childMesh->GetPolygonCount();
			for (int j = 0; j < points; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					Vertex vert;
					int index = childMesh->GetPolygonVertex(j, k);
					vert.controlPointIndice = index;

					//Vertex Data
					vert.pos[0] = (float)controlPoints[index].mData[0];
					vert.pos[1] = (float)controlPoints[index].mData[1];
					vert.pos[2] = (float)controlPoints[index].mData[2];
					vert.pos[3] = 1;

					//UV Data
					int uvi = childMesh->GetTextureUVIndex(j, k);
					FbxVector2 uvData = pUVs->GetAt(uvi);
					vert.uvs[0] = (float)(uvData.mData[0]);
					vert.uvs[1] = (float)(1.0f - uvData.mData[1]);

					//Normal Data
					vert.norms[0] = (float)normals[j * 3 + k].mData[0];
					vert.norms[1] = (float)normals[j * 3 + k].mData[1];
					vert.norms[2] = (float)normals[j * 3 + k].mData[2];
					vert.norms[3] = (float)normals[j * 3 + k].mData[3];

					verts->push_back(vert);
				}
			}

			int numDeformers = child->GetMesh()->GetDeformerCount();
			for (int d = 0; d < numDeformers; ++d)
			{
				FbxSkin* skin = (FbxSkin*)(childMesh->GetDeformer(d, FbxDeformer::eSkin));
				if (!skin)
					continue;

				FbxCluster* cluster = skin->GetCluster(0);
				FbxNode* nBone = cluster->GetLink();

				BuildHierarchy(nBone, 0, -1);

				int loop = (int)bones->size();
				for (int numBones = 0; numBones < loop; ++numBones)
				{
					if (!skin->GetCluster(numBones))
						continue;
					
						cluster = skin->GetCluster(numBones);
						FbxAMatrix c_link;
						cluster->GetTransformLinkMatrix(c_link);

						int indices = cluster->GetControlPointIndicesCount();
						double* weights = cluster->GetControlPointWeights();
						int* bIndices = cluster->GetControlPointIndices();
						for (int ind = 0; ind < indices; ++ind)
						{
							for (int l = 0; l < verts->size(); ++l)
							{
								if (verts->at(l).controlPointIndice == cluster->GetControlPointIndices()[ind])
								{
									verts->at(l).blendWeights.push_back((float)weights[ind]);
									verts->at(l).boneIndices.push_back(numBones);
								}
							}
						}

						BuildMatrix(&bones->at(numBones).transform, c_link);
						//bones->at(numBones).Translation[0] = (float)c_link.GetT().mData[0];
						//bones->at(numBones).Translation[1] = (float)c_link.GetT().mData[1];
						//bones->at(numBones).Translation[2] = (float)c_link.GetT().mData[2];
						//bones->at(numBones).Translation[3] = 1.0f;
						//bones->at(numBones).Rotation[0] = (float)c_link.GetR().mData[0];
						//bones->at(numBones).Rotation[1] = (float)c_link.GetR().mData[1];
						//bones->at(numBones).Rotation[2] = (float)c_link.GetR().mData[2];
						//bones->at(numBones).Rotation[3] = (float)c_link.GetR().mData[3];
				}

				FbxAnimStack* anim = (FbxAnimStack*)scene->GetSrcObject(FbxCriteria::ObjectType(FbxAnimStack::ClassId), 0);
				FbxTakeInfo* takeInfo = scene->GetTakeInfo(anim->GetName());
				
				for (int clusters = 0; clusters < skin->GetClusterCount(); ++clusters)
				{
					FbxCluster* cluster = skin->GetCluster(clusters);
					FbxNode* bone = cluster->GetLink();
					FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();
					FbxAnimLayer* layer = (FbxAnimLayer*)anim->GetMember(0);
					FbxTime endTime;
					FbxAnimCurve* t = bone->LclTranslation.GetCurve(layer, false);
					FbxAnimCurve* r = bone->LclRotation.GetCurve(layer, false);
					if (t)
						endTime = t->KeyGetTime(t->KeyGetCount() - 1);
					else if(r)
						endTime = r->KeyGetTime(r->KeyGetCount() - 1);

					float totalTime = (float)endTime.GetFrameCount(FbxTime::eFrames24) - startTime.GetFrameCount(FbxTime::eFrames24) + 1;
					animation->loopTime = totalTime;
					animation->bones.push_back(&bones->at(clusters));
					long long asifnaeiof = endTime.GetFrameCount(FbxTime::eFrames24);
					long long aionfwuirfibwef = startTime.GetFrameCount(FbxTime::eFrames24);
					bones->at(clusters).keyframes = new std::vector<KeyFrame*>();
					for (long long i = startTime.GetFrameCount(FbxTime::eFrames24); i < endTime.GetFrameCount(FbxTime::eFrames24); ++i)
					{
						FbxTime currentTime;
						KeyFrame* frame = new KeyFrame();
						currentTime.SetFrame(i, FbxTime::eFrames24);
						FbxAMatrix currTransform = root->EvaluateGlobalTransform(currentTime).Inverse() * bone->EvaluateGlobalTransform(currentTime);
						BuildMatrix(&frame->transform, currTransform);
						frame->keyTime = (float)currentTime.GetSecondDouble();
				
						bones->at(clusters).keyframes->push_back(frame);
					}
				}
			}
		}
	}
	else 
		return;

		std::string cut(path);

		size_t lastindex = cut.find_last_of(".");
		std::string rawname = cut.substr(0, lastindex);
		rawname.append(".bin");
	std::ofstream file(rawname, std::ios_base::binary);
	if (file.is_open())
	{
		//Number of Verts
		unsigned int numVerts = (unsigned int)verts->size();
		file.write((char*)&numVerts, sizeof(unsigned int));
	
		//For every Vert
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			//Pos
			file.write((char*)&verts->at(i).pos, sizeof(float[4]));

			//UVs
			file.write((char*)&verts->at(i).uvs, sizeof(float[2]));

			//Norms
			file.write((char*)&verts->at(i).norms, sizeof(float[4]));
	
			//Number of Blend Weights
			unsigned int numWeights = (unsigned int)verts->at(i).blendWeights.size();
			file.write((char*)&numWeights, sizeof(unsigned int));
			
			//Blend Weights
			for (unsigned int w = 0; w < numWeights; ++w)
			{
				//Blend Weights
				float temp = verts->at(i).blendWeights.at(w);
				file.write((char*)&temp, sizeof(float));
			}

			//Number of Bone Indices
			unsigned int numIndices = (unsigned int)verts->at(i).boneIndices.size();
			file.write((char*)&numIndices, sizeof(unsigned int));

			//For every Bone index
			for (unsigned int b = 0; b < numIndices; ++b)
			{
				//Bone Indices
				int temp = verts->at(i).boneIndices.at(b);
				file.write((char*)&temp, sizeof(int));
			}

			//Where controlPointIndex would be written, but it's not needed
		}

		//Number of Bones
		unsigned int numBones = (unsigned int)bones->size();
		file.write((char*)&numBones, sizeof(unsigned int));

		for (unsigned int i = 0; i < numBones; ++i)
		{
			//Current Bone's index
			file.write((char*)&bones->at(i).myIndex, sizeof(int));

			//Current Bone's parent index
			file.write((char*)&bones->at(i).parentIndex, sizeof(int));

			//Bind pos
			file.write((char*)&bones->at(i).transform, sizeof(DirectX::XMFLOAT4X4));

			//Number of Keyframes
			unsigned int numKeys = (unsigned int)bones->at(i).keyframes->size();

			file.write((char*)&numKeys, sizeof(unsigned int));

			//For each Keyframe
			for (unsigned int k = 0; k < numKeys; ++k)
			{
				//Keyframes
				file.write((char*)bones->at(i).keyframes->at(k), sizeof(KeyFrame));
			}
		}

		//Animation Time
		file.write((char*)&animation->loopTime, sizeof(float));
		file.close();

		verts->clear();
		bones->clear();
		delete verts;
		delete bones;
		delete animation;
	}
}