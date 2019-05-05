# pg_variant_tools
efficient functions for variant analyses over TINYINT[] genotype representations

<h3>Description</h3>
<p>This is a work-in-progress to develop functions for efficiently computing summary variant information over an array of genotypes stored in <code>TINYINT[]</code>. At the moment, it is little more than a collection of a couple of functions that I have used for this purpose and not the development of a function library per se.

<p>
	Currently included are:
	<ul>
		<li><code>alternate_allele_frequency( TINYINT[], INTEGER[] )</code> - a function over an array of genotypes and a corresponding array of indices into that array we should use</li>
		<li><code>summarize_variant( TINYINT[], INTEGER[], INTEGER[] )</code>a function over an array of genotypes and two corresponding array of indices into that array we should use, both of which can be null</li>
	</ul>
</p>

<p>
	The output format for summarize_variant is as a <code>variant_summary</code> user-defined type, which has fields:
	<ul>
		<li>whole genotype array call rate</li>
		<li>whole genotype array minor allele frequency</li>
		<li>first subset call rate if first subset index array is not <code>NULL</code>, otherwise <code>NULL</code></li>
		<li>first subset minor allele frequency if first subset index array is not <code>NULL</code>, otherwise <code>NULL</code></li>
		<li>second sbuset call rate if second subset index array is not <code>NULL</code>, otherwise <code>NULL</code></li>
		<li>second subset minor allele frequency if second subset index array is not <code>NULL</code>, otherwise <code>NULL</code></li>
	</ul>
</p>
<p>The interfaces beg for improvement with these functions. The summarize_variant function should be variadic on the number of subsets to accept. This I will perhaps do if there is not a large penalty to variadic argument construction with large arrays. It would be ideal for both functions to operate modularly with array_multi_index, but this requires additional scans of potentially very large arrays and sacrifices too much computational effort. Finally, returning an array would be more flexible than returning a tuple as a custom data type, which is particularly necessary if I develop a variadic function.</p>

<h3>Installation</h3>
<pre>
curl -s -S -L https://github.com/rlichtenwalter/pg_variant_functions/archive/master.zip > pg_variant_functions.zip
unzip pg_variant_functions.zip
(cd pg_variant_functions-master &amp;&amp; make PG_CONFIG=&lt;optional custom pg_config path&gt;)
(cd pg_variant_functions-master &amp;&amp; make PG_CONFIG=&lt;optional custom pg_config path&gt; install)
(cd ~postgres &amp;&amp; sudo -u postgres psql -c 'CREATE EXTENSION pg_variant_functions;')
</pre>

<h3>Usage</h3>
<pre>
</pre>
