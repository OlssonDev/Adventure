// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailPropertyRow.h"

class FObjectiveDetailCustomization : public IPropertyTypeCustomization
{
	public:

	FObjectiveDetailCustomization();

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FObjectiveDetailCustomization);
	}

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	// End of IPropertyTypeCustomization interface

	private:

	void OnCheckBoxNewState(ECheckBoxState NewState);
	ECheckBoxState GetCheckBoxState() const;

	private:

	TSharedPtr<IPropertyHandle> bUseTagAsArgumentProperty;
	TSharedPtr<IPropertyHandle> ArgumentNameProperty;
	TSharedPtr<IPropertyHandle> ArgumentTagProperty;

	/** Cached property handle */
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	IPropertyTypeCustomizationUtils* StructCustomization;
};
