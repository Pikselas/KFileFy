# KFileFy
file sharing with c++
<h2>Simple C++ <i>(also unplanned and buggy and people don't want these types of things)</i> File sharing Software</h2>
<h3>Sadly only works in windows</h3>
<h4>Created using winsock2 </h4>
<h1>UNDER CONSTRUCTION</h1>
<h2>To do</h2>
<ol>
<li>
 Design proper Ks_FileReceiver for file receiveing
</li>
 <li>
   check if newly added port is already there 
 </li>
 <li>
   add receive option
 </li>
 <li>
  file status => use enum instead of bool
 </li>
 <li>
   Sending cancellation
 </li>
 <li>
   Total file size calculation
 </li>
 <li>
 </li>
</ol>
<h2>MISTAKES YOU SHOULD NEVER DO</h2>
<ul>
 <li>Always build up seperate class if it can be done (don't do things like this idiot did for Ks_Connector, always design seperate class for things like Server and Client)</li>
 <li>Must use Inhertiance if classes shares same features (or you will suffer like this a**l did)</li>
</ul>
