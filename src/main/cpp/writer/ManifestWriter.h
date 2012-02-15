/*
 * ManifestWriter.h
 *
 *  Created on: Feb 15, 2012
 *      Author: ralf
 */

#ifndef MANIFESTWRITER_H_
#define MANIFESTWRITER_H_

#include "../modules/BasicModule.h"

class ManifestWriter : public BasicModule {
public:
	ManifestWriter(const string& productId);
	~ManifestWriter();

	void start(Context& context);
	void stop(Context& context);

private:
	void copyTemplateFiles() const;
	string readManifest() const;
	void setStartTime(const Context& context, string& manifest) const;
	void setChecksums(string& manifest) const;
	void writeManifest(string& manifest) const;
	void removeManifestTemplate() const;

	static string getMd5Sum(const string& file);

	const string productId;
	const string manifestName;

	path targetDirPath;
};

#endif /* MANIFESTWRITER_H_ */
