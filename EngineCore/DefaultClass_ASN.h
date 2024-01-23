#pragma once

class DefaultClass_ASN
{
public:
	// constrcuter destructer
	DefaultClass_ASN();
	~DefaultClass_ASN();

	// delete Function
	DefaultClass_ASN(const DefaultClass_ASN& _Other) = delete;
	DefaultClass_ASN(DefaultClass_ASN&& _Other) noexcept = delete;
	DefaultClass_ASN& operator=(const DefaultClass_ASN& _Other) = delete;
	DefaultClass_ASN& operator=(DefaultClass_ASN&& _Other) noexcept = delete;

protected:

private:

};