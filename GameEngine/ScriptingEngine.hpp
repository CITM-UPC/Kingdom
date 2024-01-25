#pragma once
#include <iostream>
#include <fstream>
#include "Engine_Globals.h"
#include "..\mono\include\mono\jit\jit.h"
#include "..\mono\include\mono\metadata\assembly.h"
#include "..\mono\include\mono\metadata\attrdefs.h"


class ScriptingEngine
{
    enum class Accessibility : uint8_t
    {
        None = 0,
        Private = (1 << 0),
        Internal = (1 << 1),
        Protected = (1 << 2),
        Public = (1 << 3)
    };

public:
	MonoDomain* monoRootDomain = NULL;
	MonoDomain* monoAppDomain = NULL;

	void InitMono()
	{
		mono_set_assemblies_path("../mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("MyScriptRuntime");

		if (rootDomain == nullptr)
		{
			// Maybe log some error here
			std::cout << "MonoDomain has not been initialised correctly." << std::endl;
			return;
		}

		// Store the root domain pointer
		monoRootDomain = rootDomain;

		char appDomainName[] = "MyAppDomain";
		monoAppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
		mono_domain_set(monoAppDomain, true);
	}

    char* ReadBytes(const std::string& filepath, uint32_t* outSize)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            // Failed to open the file
            std::cout << "Failed to open file." << std::endl;
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();

        if (size == 0)
        {
            // File is empty
            std::cout << "File is empty" << std::endl;
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        *outSize = size;
        return buffer;
    }

    MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
    {
        uint32_t fileSize = 0;
        char* fileData = ReadBytes(assemblyPath, &fileSize);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            // Log some error message using the errorMessage data
            std::cout << "Mono loading assembly error: " << errorMessage << std::endl;
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
        mono_image_close(image);

        // Don't forget to free the file data
        delete[] fileData;

        return assembly;
    }

    void PrintAssemblyTypes(MonoAssembly* assembly)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            std::cout << "NameSpace: " << nameSpace << ". Name: " << name << std::endl;
        }
    }

    MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        MonoClass* cSharpClass = mono_class_from_name(image, namespaceName, className);

        if (cSharpClass == nullptr)
        {
            // Log error here
            std::cout << "Could not find C# class " << className << std::endl;
            return nullptr;
        }

        return cSharpClass;
    }

    MonoObject* InstantiateClass(const std::string& assemblyPath, const char* namespaceName, const char* className)
    {
        MonoAssembly* assembly = LoadCSharpAssembly(assemblyPath);

        // Get a reference to the class we want to instantiate
        MonoClass* testingClass = GetClassInAssembly(assembly, namespaceName, className);

        // Allocate an instance of our class
        MonoObject* classInstance = mono_object_new(monoAppDomain, testingClass);

        if (classInstance == nullptr)
        {
            // Log error here and abort
            std::cout << "Could not create instance of C# class " << className << std::endl;
            return nullptr;
        }

        // Call the parameterless (default) constructor
        mono_runtime_object_init(classInstance);
        std::cout << "Instance of " << className << "created and initialized." << std::endl;

        PrintAssemblyTypes(assembly);

        return classInstance;
    }

    void CallPrintFloatVarMethod(MonoObject* objectInstance)
    {
        // Get the MonoClass pointer from the instance
        MonoClass* instanceClass = mono_object_get_class(objectInstance);

        // Get a reference to the method in the class
        MonoMethod* method = mono_class_get_method_from_name(instanceClass, "PrintFloatVar", 0);

        if (method == nullptr)
        {
            // No method called "PrintFloatVar" with 0 parameters in the class, log error or something
            std::cout << "Could not find method PrintFloatVar" << std::endl;
            return;
        }

        // Call the C# method on the objectInstance instance, and get any potential exceptions
        MonoObject* exception = nullptr;
        mono_runtime_invoke(method, objectInstance, nullptr, &exception);

        //Handle the exception
        if (exception != nullptr)
        {
            std::cout << "Exception occurred" << std::endl;
            return;
        }
    }

    void CallIncrementFloatVarMethod(MonoObject* objectInstance, float value)
    {
        // Get the MonoClass pointer from the instance
        MonoClass* instanceClass = mono_object_get_class(objectInstance);

        // Get a reference to the method in the class
        MonoMethod* method = mono_class_get_method_from_name(instanceClass, "IncrementFloatVar", 1);

        if (method == nullptr)
        {
            // No method called "IncrementFloatVar" with 1 parameter in the class, log error or something
            std::cout << "Could not find method IncrementFloatVar" << std::endl;
            return;
        }

        // Call the C# method on the objectInstance instance, and get any potential exceptions
        //MonoObject* exception = nullptr;
        //void* param = &value;
        //mono_runtime_invoke(method, objectInstance, &param, &exception);

        // OR

        MonoObject* exception = nullptr;
        void* params[] =
        {
            &value
        };

        mono_runtime_invoke(method, objectInstance, params, &exception);

        // TODO: Handle the exception
        if (exception != nullptr)
        {
            std::cout << "Exception occurred" << std::endl;
            return;
        }
    }

    // Gets the accessibility level of the given field
    uint8_t GetFieldAccessibility(MonoClassField* field)
    {
        uint8_t accessibility = (uint8_t)Accessibility::None;
        uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

        switch (accessFlag)
        {
        case MONO_FIELD_ATTR_PRIVATE:
        {
            accessibility = (uint8_t)Accessibility::Private;
            break;
        }
        case MONO_FIELD_ATTR_FAM_AND_ASSEM:
        {
            accessibility |= (uint8_t)Accessibility::Protected;
            accessibility |= (uint8_t)Accessibility::Internal;
            break;
        }
        case MONO_FIELD_ATTR_ASSEMBLY:
        {
            accessibility = (uint8_t)Accessibility::Internal;
            break;
        }
        case MONO_FIELD_ATTR_FAMILY:
        {
            accessibility = (uint8_t)Accessibility::Protected;
            break;
        }
        case MONO_FIELD_ATTR_FAM_OR_ASSEM:
        {
            accessibility |= (uint8_t)Accessibility::Private;
            accessibility |= (uint8_t)Accessibility::Protected;
            break;
        }
        case MONO_FIELD_ATTR_PUBLIC:
        {
            accessibility = (uint8_t)Accessibility::Public;
            break;
        }
        }

        return accessibility;
    }

    // Gets the accessibility level of the given property
    uint8_t GetPropertyAccessibility(MonoProperty* property)
    {
        uint8_t accessibility = (uint8_t)Accessibility::None;

        // Get a reference to the property's getter method
        MonoMethod* propertyGetter = mono_property_get_get_method(property);
        if (propertyGetter != nullptr)
        {
            // Extract the access flags from the getters flags
            uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

            switch (accessFlag)
            {
            case MONO_FIELD_ATTR_PRIVATE:
            {
                accessibility = (uint8_t)Accessibility::Private;
                break;
            }
            case MONO_FIELD_ATTR_FAM_AND_ASSEM:
            {
                accessibility |= (uint8_t)Accessibility::Protected;
                accessibility |= (uint8_t)Accessibility::Internal;
                break;
            }
            case MONO_FIELD_ATTR_ASSEMBLY:
            {
                accessibility = (uint8_t)Accessibility::Internal;
                break;
            }
            case MONO_FIELD_ATTR_FAMILY:
            {
                accessibility = (uint8_t)Accessibility::Protected;
                break;
            }
            case MONO_FIELD_ATTR_FAM_OR_ASSEM:
            {
                accessibility |= (uint8_t)Accessibility::Private;
                accessibility |= (uint8_t)Accessibility::Protected;
                break;
            }
            case MONO_FIELD_ATTR_PUBLIC:
            {
                accessibility = (uint8_t)Accessibility::Public;
                break;
            }
            }
        }

        // Get a reference to the property's setter method
        MonoMethod* propertySetter = mono_property_get_set_method(property);
        if (propertySetter != nullptr)
        {
            // Extract the access flags from the setters flags
            uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
            if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
                accessibility = (uint8_t)Accessibility::Private;
        }
        else
        {
            accessibility = (uint8_t)Accessibility::Private;
        }

        return accessibility;
    }

    void LetsTestSomeThings()
    {
        MonoObject* testInstance = InstantiateClass("../ScriptingSandbox/bin/Debug/ScriptingSandbox.dll", "", "CSharpTesting");
        CallPrintFloatVarMethod(testInstance);
        CallIncrementFloatVarMethod(testInstance, 1.0f);
        CallPrintFloatVarMethod(testInstance);
        CallIncrementFloatVarMethod(testInstance, 2.0f);
        CallPrintFloatVarMethod(testInstance);
        CallIncrementFloatVarMethod(testInstance, 7.0f);
        CallPrintFloatVarMethod(testInstance);


        //Testing get fields
        
        // Get the MonoClass pointer from the instance
        MonoClass* instanceClass = mono_object_get_class(testInstance);
        // Get a reference to the public field called "MyPublicFloatVar"
        MonoClassField* floatField = mono_class_get_field_from_name(instanceClass, "MyPublicFloatVar");
        uint8_t floatFieldAccessibility = GetFieldAccessibility(floatField);

        if (floatFieldAccessibility & (uint8_t)Accessibility::Public)
        {
            // We can safely write a value to this
            std::cout << "Reading or writing public field directly " << floatField << std::endl;
        }

        // Get a reference to the private field called "m_Name"
        MonoClassField* nameField = mono_class_get_field_from_name(instanceClass, "m_Name");
        uint8_t nameFieldAccessibility = GetFieldAccessibility(nameField);

        if (nameFieldAccessibility & (uint8_t)Accessibility::Private)
        {
            // We shouldn't write to this field
            std::cout << "Should not write to private field " << nameField << std::endl;
        }

        // Get a reference to the public property called "Name"
        MonoProperty* nameProperty = mono_class_get_property_from_name(instanceClass, "Name");
        uint8_t namePropertyAccessibility = GetPropertyAccessibility(nameProperty);

        if (namePropertyAccessibility & (uint8_t)Accessibility::Public)
        {
            // We can safely write a value to the field using this property
            std::cout << "Reading or writing public property directly " << nameProperty << std::endl;
        }

        // Do something
    }
};