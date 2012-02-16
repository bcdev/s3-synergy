/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef MANIFESTWRITER_H_
#define MANIFESTWRITER_H_

#include "../modules/BasicModule.h"

/**
 * The module used for writing the SAFE manifest.
 */
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
