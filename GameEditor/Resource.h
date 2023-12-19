#pragma once
#include <string>
#include "Globals.h"

class Resource
{
public:
	enum Type {
		TEXTURE,
		MESH,
		//AUDIO,
		//SCENE,
		//BONE,
		//ANIMATION,
		UNKNOWN
	};

	//Resource(UID uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const;
	//UID GetUID() const;
	const char* GetAssetFile() const;
	const char* GetLibraryFile() const;

	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint GetReferenceCount() const;

	/*virtual void Save(Config& config) const;
	virtual void Load(const Config& config);*/
	virtual bool LoadInMemory() = 0;

protected:
	//UID uid = 0;
	std::string assetsFile;
	std::string Libraryile;

	Type type = UNKNOWN;
	uint referenceCount = 0;
};
